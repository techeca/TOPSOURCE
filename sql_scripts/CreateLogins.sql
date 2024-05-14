USE [master];
CREATE LOGIN pko_account WITH PASSWORD = 'Y87dc#$98', DEFAULT_DATABASE = AccountServer;
CREATE LOGIN pko_game WITH PASSWORD = 'Y87dc#$98', DEFAULT_DATABASE = GameDB;
USE [AccountServer];
CREATE USER pko_account FROM LOGIN pko_account;
ALTER ROLE db_owner ADD MEMBER pko_account;
USE [GameDB];
CREATE USER pko_game FROM LOGIN pko_game;
ALTER ROLE db_owner ADD MEMBER pko_game;