#ifndef TOTPGENERATOR_H
#define TOTPGENERATOR_H

#include <QString>

class TOTPGenerator {
public:
    TOTPGenerator(const QString secret);

    QString generateCode();
private:
    QByteArray decodeBase32(const QString &);
    QByteArray hmacSha1(QByteArray key, QByteArray baseString);

    const QString secret;
};

#endif // TOTPGENERATOR_H
