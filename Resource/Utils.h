//
// Created by lucas on 09/10/2020.
//
#include "bits/stdc++.h"

#ifndef LAB2CES35_UTILS_H
#define LAB2CES35_UTILS_H

std::size_t readLine(int socket, std::string & buffer, std::size_t maxLen);
std::string trim(const std::string & str);
void writeLine(int socket, std::string & str);
void outputHttpHeaders(int conn, int status, std::size_t size);
std::string getStatusMessage(int status);


#endif //LAB2CES35_UTILS_H
