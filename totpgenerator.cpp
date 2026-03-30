#include "totpgenerator.h"

#include <QDateTime>
#include <QtEndian>
#include <QCryptographicHash>

TOTPGenerator::TOTPGenerator(const QString secret) : secret(secret)
{
}

QByteArray TOTPGenerator::decodeBase32(const QString &secret)
{
    int buffer = 0;
    int bitsLeft = 0;
    QByteArray array;

    for (const QChar &chr : secret)
    {
        int ch = chr.toLatin1();

        if(ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '-')
        {
            continue;
        }

        buffer <<= 5;

        // Deal with commonly mistyped characters
        if(ch == '0')
        {
            ch = 'O';
        } else if(ch == '1')
        {
            ch = 'L';
        } else if(ch == '8')
        {
            ch = 'B';
        }

        // Look up one base32 digit
        if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            ch = (ch & 0x1F) - 1;
        } else if(ch >= '2' && ch <= '7')
        {
            ch -= '2' - 26;
        }
        else
        {
            return array;
        }

        buffer |= ch;
        bitsLeft += 5;

        if(bitsLeft >= 8)
        {
            array.append(buffer >> (bitsLeft - 8));
            bitsLeft -= 8;
        }
    }

    return array;
}

QByteArray TOTPGenerator::hmacSha1(QByteArray key, QByteArray baseString)
{
    int blockSize = 64;

    if(key.length() > blockSize)
    {
        key = QCryptographicHash::hash(key, QCryptographicHash::Sha1);
    }

    QByteArray innerPadding(blockSize, char(0x36));
    QByteArray outerPadding(blockSize, char(0x5c));

    for(int i = 0; i < key.length(); ++i)
    {
        innerPadding[i] = innerPadding[i] ^ key.at(i);
        outerPadding[i] = outerPadding[i] ^ key.at(i);
    }

    QByteArray total = outerPadding;
    QByteArray part = innerPadding;

    part.append(baseString);
    total.append(QCryptographicHash::hash(part, QCryptographicHash::Sha1));

    return QCryptographicHash::hash(total, QCryptographicHash::Sha1);
}

QString TOTPGenerator::generateCode()
{
    quint64 time = QDateTime::currentDateTime().toSecsSinceEpoch();
    quint64 current = qToBigEndian(time / 30);

    QByteArray res = decodeBase32(secret);
    QByteArray hmac = hmacSha1(res, QByteArray((char*)&current, sizeof(current)));

    int offset = (hmac[hmac.length() - 1] & 0xf);
    int binary =
            ((hmac[offset] & 0x7f) << 24)
            | ((hmac[offset + 1] & 0xff) << 16)
            | ((hmac[offset + 2] & 0xff) << 8)
            | (hmac[offset + 3] & 0xff);

    int password = binary % 1000000;
    return QString("%1").arg(password, 6, 10, QChar('0'));
}
