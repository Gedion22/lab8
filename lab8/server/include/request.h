#ifndef STORAGE_H
#define STORAGE_H

/** @file
    @brief request
*/


#include <string.h>
#include <ctype.h>
#include <jansson.h>
#include <progbase/net.h>

typedef struct Memes Memes;

/**
    @brief first req
    @param message
    @returns void
*/
void req1(NetMessage * message);
/**
    @brief favorite req
    @param message
    @returns void
*/
void reqFavorites(NetMessage * message);
/**
    @brief favorite req key
    @param message,key, value
    @returns void
*/
void reqFavoritesKey(int key, char* value, NetMessage * message);
/**
    @brief req file
    @param message
    @returns void
*/
void reqFile(NetMessage * message);
/**
    @brief data file req
    @param message,key
    @returns void
*/
void reqFileData(NetMessage * message);
void reqError(NetMessage * message);
#endif