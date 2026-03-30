#ifndef LAUNCHERUSER_H
#define LAUNCHERUSER_H

#include <QString>

class LauncherUser
{
public:
    LauncherUser(QString, QString, QString);

    QString getUsername() const;
    QString getPassword() const;
    QString getSecret() const;

    void setUsername(QString);
    void setPassword(QString);
    void setSecret(QString);

private:
    QString username;
    QString password;
    QString secret;
};

#endif // LAUNCHERUSER_H
