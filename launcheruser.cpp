#include "launcheruser.h"

LauncherUser::LauncherUser(QString username, QString password, QString secret)
    : username(username), password(password), secret(secret)
{
}

QString LauncherUser::getUsername() const
{
    return username;
}

QString LauncherUser::getPassword() const
{
    return password;
};

QString LauncherUser::getSecret() const
{
    return secret;
};

void LauncherUser::setUsername(QString username)
{
    this->username = username;
};

void LauncherUser::setPassword(QString password)
{
    this->password = password;
}

void LauncherUser::setSecret(QString secret)
{
    this->secret = secret;
}
