/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the Qt Purchasing Add-on.
**
** $QT_BEGIN_LICENSE$
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** contact form at http://qt.digia.com
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef HANGMANGAME_H
#define HANGMANGAME_H

#include <QObject>
#include <QStringList>
#include <QMutex>
#include <QtPurchasing>
#include <QSettings>

class HangmanGame : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString word READ word NOTIFY wordChanged)
    Q_PROPERTY(QString lettersOwned READ lettersOwned NOTIFY lettersOwnedChanged)
    Q_PROPERTY(QString vowels READ vowels CONSTANT)
    Q_PROPERTY(QString consonants READ consonants CONSTANT)
    Q_PROPERTY(int errorCount READ errorCount NOTIFY errorCountChanged)
    Q_PROPERTY(bool vowelsUnlocked READ vowelsUnlocked WRITE setVowelsUnlocked NOTIFY vowelsUnlockedChanged)
    Q_PROPERTY(int vowelsAvailable READ vowelsAvailable WRITE setVowelsAvailable NOTIFY vowelsAvailableChanged)
    Q_PROPERTY(int wordsGiven READ wordsGiven WRITE setWordsGiven NOTIFY wordsGivenChanged)
    Q_PROPERTY(int  wordsGuessedCorrectly READ wordsGuessedCorrectly WRITE setWordsGuessedCorrectly NOTIFY wordsGuessedCorrectlyChanged)
    Q_PROPERTY(int score READ score WRITE setScore NOTIFY scoreChanged)

public:
    explicit HangmanGame(QObject *parent = 0);
    Q_INVOKABLE void reset();
    Q_INVOKABLE void reveal();
    Q_INVOKABLE void gameOverReveal();
    Q_INVOKABLE void requestLetter(const QString &letterString);
    Q_INVOKABLE void guessWord(const QString &word);
    Q_INVOKABLE bool isVowel(const QString &letter);
    Q_INVOKABLE void setVowelsAvailable(int count);
    Q_INVOKABLE void setWordsGiven(int count);
    Q_INVOKABLE void setWordsGuessedCorrectly(int count);
    Q_INVOKABLE void setScore(int score);

    QString word() const { return m_word; }
    QString lettersOwned() const { return m_lettersOwned; }
    QString vowels() const;
    QString consonants() const;
    int errorCount() const;
    bool vowelsUnlocked() const;
    void setVowelsUnlocked(bool vowelsUnlocked);
    int vowelsAvailable() const;
    int wordsGiven() const;
    int wordsGuessedCorrectly() const;
    int score() const;

signals:
    void wordChanged();
    void lettersOwnedChanged();
    void errorCountChanged();
    void vowelBought(const QChar &vowel);
    void purchaseWasSuccessful(bool wasSuccessful);
    void vowelsUnlockedChanged(bool unlocked);
    void vowelsAvailableChanged(int arg);
    void wordsGivenChanged(int arg);
    void wordsGuessedCorrectlyChanged(int arg);
    void scoreChanged(int arg);

private slots:
    void registerLetterBought(const QChar &letter);

private:
    void chooseRandomWord();
    void initWordList();
    int calculateEarnedVowels();
    int calculateEarnedPoints();

    QString m_word;
    QString m_lettersOwned;
    QStringList m_wordList;
    QMutex m_lock;
    bool m_vowelsUnlocked;
    QSettings m_persistentSettings;
    int m_vowelsAvailable;
    int m_wordsGiven;
    int m_wordsGuessedCorrectly;
    int m_score;
};

#endif // HANGMANGAME_H
