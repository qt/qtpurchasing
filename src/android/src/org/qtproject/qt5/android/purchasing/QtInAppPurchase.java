/****************************************************************************
**
** Copyright (C) 2023 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Purchasing module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3-COMM$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

package org.qtproject.qt5.android.purchasing;

import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.content.Context;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.android.billingclient.api.AcknowledgePurchaseParams;
import com.android.billingclient.api.AcknowledgePurchaseResponseListener;
import com.android.billingclient.api.BillingClient;
import com.android.billingclient.api.BillingClientStateListener;
import com.android.billingclient.api.BillingFlowParams;
import com.android.billingclient.api.BillingResult;
import com.android.billingclient.api.ConsumeParams;
import com.android.billingclient.api.ConsumeResponseListener;
import com.android.billingclient.api.Purchase;
import com.android.billingclient.api.Purchase.PurchaseState;
import com.android.billingclient.api.PurchasesResponseListener;
import com.android.billingclient.api.PurchasesUpdatedListener;
import com.android.billingclient.api.QueryProductDetailsParams;
import com.android.billingclient.api.QueryPurchasesParams;
import com.android.billingclient.api.QueryProductDetailsParams.Product;
import com.android.billingclient.api.ProductDetailsResponseListener;
import com.android.billingclient.api.BillingFlowParams.ProductDetailsParams;
import com.android.billingclient.api.ProductDetails;

public class QtInAppPurchase implements PurchasesUpdatedListener
{
    private Context m_context = null;
    private String m_publicKey = null;
    private final long m_nativePointer;
    private int requestCode;

    private BillingClient billingClient;

    public static final int RESULT_OK = BillingClient.BillingResponseCode.OK;
    public static final int RESULT_USER_CANCELED = BillingClient.BillingResponseCode.USER_CANCELED;
    public static final int RESULT_BILLING_UNAVAILABLE =
            BillingClient.BillingResponseCode.BILLING_UNAVAILABLE;
    public static final int RESULT_ITEM_UNAVAILABLE =
            BillingClient.BillingResponseCode.ITEM_UNAVAILABLE;
    public static final int RESULT_DEVELOPER_ERROR =
            BillingClient.BillingResponseCode.DEVELOPER_ERROR;
    public static final int RESULT_ERROR = BillingClient.BillingResponseCode.ERROR;
    public static final int RESULT_ITEM_ALREADY_OWNED =
            BillingClient.BillingResponseCode.ITEM_ALREADY_OWNED;
    public static final int RESULT_ITEM_NOT_OWNED =
        BillingClient.BillingResponseCode.ITEM_NOT_OWNED;
    // No match with any already defined response codes
    public static final int RESULT_QTPURCHASING_ERROR = 9;

    public static final String TAG = "QtInAppPurchase";
    public static final String TYPE_INAPP = BillingClient.ProductType.INAPP;
    public static final int IAP_VERSION = 3;

    // Should be in sync with QInAppTransaction::FailureReason
    public static final int FAILUREREASON_NOFAILURE    = 0;
    public static final int FAILUREREASON_USERCANCELED = 1;
    public static final int FAILUREREASON_ERROR        = 2;

    public QtInAppPurchase(Context context, long nativePointer) {
        m_context = context;
        m_nativePointer = nativePointer;
    }

    public void initializeConnection() {
        billingClient = BillingClient.newBuilder(m_context).enablePendingPurchases()
                .setListener(this).build();
        billingClient.startConnection(new BillingClientStateListener() {
            @Override
            public void onBillingSetupFinished(BillingResult billingResult) {
                if (billingResult.getResponseCode() == RESULT_OK) {
                    queryPurchasedProducts();
                }
            }
            @Override
            public void onBillingServiceDisconnected() {
                Log.w(TAG, "Billing service disconnected");
            }
        });
    }

    @Override
    public void onPurchasesUpdated(@NonNull BillingResult billingResult,
            @Nullable List<Purchase> list) {
        int responseCode = billingResult.getResponseCode();
        if (list == null || list.isEmpty()) {
            purchaseFailed(requestCode, FAILUREREASON_ERROR, "Data missing from result");
            return;
        }
        if (responseCode == RESULT_USER_CANCELED) {
            purchaseFailed(requestCode, FAILUREREASON_USERCANCELED, "Purchase Canceled.");
            return;
        } else if (responseCode != RESULT_OK) {
            String errorString;
            switch (responseCode) {
                case RESULT_BILLING_UNAVAILABLE: errorString = "Billing unavailable"; break;
                case RESULT_ITEM_UNAVAILABLE: errorString = "Item unavailable"; break;
                case RESULT_DEVELOPER_ERROR: errorString = "Developer error"; break;
                case RESULT_ERROR: errorString = "Fatal error occurred"; break;
                case RESULT_ITEM_ALREADY_OWNED: errorString = "Item already owned"; break;
                default: errorString = "Unknown billing error " + responseCode; break;
            };
            purchaseFailed(requestCode, FAILUREREASON_ERROR, errorString);
            return;
        }
        for (Purchase purchase : list) {
            try {
                if (m_publicKey != null && !Security.verifyPurchase(m_publicKey,
                        purchase.getOriginalJson(), purchase.getSignature())) {
                    purchaseFailed(requestCode, FAILUREREASON_ERROR,
                            "Signature could not be verified");
                    return;
                }
                int purchaseState = purchase.getPurchaseState();
                if (purchaseState != PurchaseState.PURCHASED) {
                    purchaseFailed(requestCode, FAILUREREASON_ERROR,
                            "Unexpected purchase state in result");
                    return;
                }
            } catch (Exception e) {
                e.printStackTrace();
                purchaseFailed(requestCode, FAILUREREASON_ERROR, e.getMessage());
            }
            purchaseSucceeded(requestCode,
                    purchase.getSignature(),
                    purchase.getOriginalJson(),
                    purchase.getPurchaseToken(),
                    purchase.getOrderId(),
                    purchase.getPurchaseTime());
        }
    }

    public void queryPurchasedProducts() {
        billingClient.queryPurchasesAsync(QueryPurchasesParams.newBuilder()
                .setProductType(TYPE_INAPP).build(),new PurchasesResponseListener() {
            public void onQueryPurchasesResponse(BillingResult billingResult,
                    List<Purchase> purchases) {
                int responseCode = billingResult.getResponseCode();
                if (responseCode != RESULT_OK) {
                    Log.e(TAG, "queryPurchasedProducts: Failed to query purchase");
                    return;
                }
                if (purchases == null || purchases.isEmpty()) {
                    Log.e(TAG, "queryPurchasedProducts: No purchase list in response.");
                    return;
                }
                for (Purchase purchase : purchases) {
                    String signature = purchase.getSignature();
                    if (signature == null || signature.isEmpty()) {
                        Log.e(TAG, "queryPurchasedProducts: No Signature in purchase");
                        continue;
                    }
                    registerPurchased(m_nativePointer,
                            purchase.getProducts().get(0),
                            purchase.getSignature(),
                            purchase.getOriginalJson(),
                            purchase.getPurchaseToken(),
                            purchase.getDeveloperPayload(),
                            purchase.getPurchaseTime());
                }
                purchasedProductsQueried(m_nativePointer);
            }
        });
    }

    public void queryDetails(final String[] productIds) {
        List<Product> productIdList = new ArrayList<>();
        for (String productId : productIds) {
            productIdList.add(Product.newBuilder().setProductId(productId)
                    .setProductType(TYPE_INAPP).build());
        }
        QueryProductDetailsParams queryProductDetailsParams =
                QueryProductDetailsParams.newBuilder().setProductList(productIdList).build();
        billingClient.queryProductDetailsAsync(queryProductDetailsParams,
                new ProductDetailsResponseListener() {
            @Override
            public void onProductDetailsResponse(BillingResult billingResult,
                    List<ProductDetails> productDetailsResponseList) {
                int responseCode = billingResult.getResponseCode();
                if (responseCode != RESULT_OK) {
                    Log.e(TAG, "queryDetails: Couldn't retrieve product details.");
                }
                if (productDetailsResponseList == null || productDetailsResponseList.isEmpty()) {
                    Log.e(TAG, "queryDetails: No details list in response.");
                }
                final ArrayList<String> failedProducts = new ArrayList<>();
                for (ProductDetails productDetails : productDetailsResponseList) {
                    try {
                        String queriedProductId = productDetails.getProductId();
                        String queriedPrice = productDetails.getOneTimePurchaseOfferDetails()
                                .getFormattedPrice();
                        String queriedTitle = productDetails.getTitle();
                        String queriedDescription = productDetails.getDescription();
                        if (queriedProductId.equals("") ||
                                queriedPrice.equals("") ||
                                queriedTitle.equals("") ||
                                queriedDescription.equals("")) {
                            Log.e(TAG, "Data missing from product details.");
                            failedProducts.add(queriedProductId);
                        } else {
                            registerProduct(m_nativePointer,
                                    queriedProductId,
                                    queriedPrice,
                                    queriedTitle,
                                    queriedDescription);
                        }
                        if (productDetailsResponseList.size() ==
                                productDetailsResponseList.indexOf(productDetails) + 1){
                            for (String failedProduct : failedProducts)
                                queryFailed(m_nativePointer, failedProduct);
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                        Log.e(TAG, "queryDetails: ", e);
                    }
                }
            }
        });
    }

    public void setPublicKey(String publicKey) {
        m_publicKey = publicKey;
    }

    public void launchBillingFlow(String identifier, int purchaseRequestCode) {
        List<Product> productIdList = new ArrayList<>();
        productIdList.add(Product.newBuilder().setProductId(identifier).setProductType(TYPE_INAPP)
                .build());
        QueryProductDetailsParams queryProductDetailsParams = QueryProductDetailsParams
                .newBuilder().setProductList(productIdList).build();
        billingClient.queryProductDetailsAsync(queryProductDetailsParams,
                new ProductDetailsResponseListener() {
            @Override
            public void onProductDetailsResponse(BillingResult billingResult,
                    List<ProductDetails> productDetailsResponseList) {
                int responseCode = billingResult.getResponseCode();
                if (responseCode != RESULT_OK) {
                    Log.e(TAG, "Unable to launch the purchase flow. Response code: "
                            + responseCode);
                    String errorString;
                    switch (responseCode) {
                        case RESULT_BILLING_UNAVAILABLE: errorString = "Billing unavailable"; break;
                        case RESULT_ITEM_UNAVAILABLE: errorString = "Item unavailable"; break;
                        case RESULT_DEVELOPER_ERROR: errorString = "Developer error"; break;
                        case RESULT_ERROR: errorString = "Fatal error occurred"; break;
                        case RESULT_ITEM_ALREADY_OWNED: errorString = "Item already owned"; break;
                        default: errorString = "Unknown billing error " + responseCode; break;
                    };
                    purchaseFailed(requestCode, FAILUREREASON_ERROR, errorString);
                    return;
                }else if (productDetailsResponseList == null
                        || productDetailsResponseList.isEmpty()) {
                    purchaseFailed(requestCode, FAILUREREASON_ERROR, "Data missing from result");
                    return;
                }
                List<ProductDetailsParams> productDetailsParamsList = new ArrayList<>();
                productDetailsParamsList.add(ProductDetailsParams.newBuilder()
                        .setProductDetails(productDetailsResponseList.get(0)).build()
                );
                BillingFlowParams billingFlowParams = BillingFlowParams.newBuilder()
                        .setProductDetailsParamsList(productDetailsParamsList).build();
                billingClient.launchBillingFlow((Activity) m_context, billingFlowParams);
            }
        });
    }

    public void consumePurchase(String purchaseToken) {
        ConsumeResponseListener listener = new ConsumeResponseListener() {
            @Override
            public void onConsumeResponse(BillingResult billingResult, String purchaseToken) {
                if (billingResult.getResponseCode() != RESULT_OK) {
                    Log.e(TAG, "Unable to consume purchase. Response code: "
                            + billingResult.getResponseCode());
                    return;
                }
            }
        };
        ConsumeParams consumeParams = ConsumeParams.newBuilder().setPurchaseToken(purchaseToken)
                .build();
        billingClient.consumeAsync(consumeParams, listener);
    }

    public void acknowledgePurchase(String purchaseToken) {
        AcknowledgePurchaseParams acknowledgePurchaseParams = AcknowledgePurchaseParams.newBuilder()
                .setPurchaseToken(purchaseToken).build();
        AcknowledgePurchaseResponseListener acknowledgePurchaseResponseListener =
                new AcknowledgePurchaseResponseListener() {
            @Override
            public void onAcknowledgePurchaseResponse(BillingResult billingResult) {
                if (billingResult.getResponseCode() != RESULT_OK){
                    Log.e(TAG, "Unable to acknowledge purchase. Response code: "
                            + billingResult.getResponseCode());
                    return;
                }
            }
        };
        billingClient.acknowledgePurchase(acknowledgePurchaseParams,
                acknowledgePurchaseResponseListener);
    }

    private void purchaseFailed(int requestCode, int failureReason, String errorString) {
        purchaseFailed(m_nativePointer, requestCode, failureReason, errorString);
    }

    private void purchaseSucceeded(int requestCode,
                                   String signature,
                                   String purchaseData,
                                   String purchaseToken,
                                   String orderId,
                                   long timestamp) {
        purchaseSucceeded(m_nativePointer,
                          requestCode,
                          signature,
                          purchaseData,
                          purchaseToken,
                          orderId,
                          timestamp);
    }

    private native static void queryFailed(long nativePointer, String productId);
    private native static void purchasedProductsQueried(long nativePointer);
    private native static void registerProduct(long nativePointer,
                                               String productId,
                                               String price,
                                               String title,
                                               String description);
    private native static void purchaseFailed(long nativePointer,
                                              int requestCode,
                                              int failureReason,
                                              String errorString);
    private native static void purchaseSucceeded(long nativePointer,
                                                 int requestCode,
                                                 String signature,
                                                 String data,
                                                 String purchaseToken,
                                                 String orderId,
                                                 long timestamp);
    private native static void registerPurchased(long nativePointer,
                                                 String identifier,
                                                 String signature,
                                                 String data,
                                                 String purchaseToken,
                                                 String orderId,
                                                 long timestamp);
}
