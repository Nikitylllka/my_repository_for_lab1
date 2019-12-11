// Copyright 2018 Your Name <your_email>

#include "header.hpp"


int main() {
    Json test;

    Json hello("{\n"
               "    \"lastname\" : \"Ivanov\",\n"
               "    \"firstname\" : \"Ivan\",\n"
               "    \"age\" : 25,\n"
               "    \"islegal\" : false,\n"
               "    \"marks\" : [\n"
               "    \t4,5,5,5,2,3\n"
               "    ],\n"
               "    \"address\" : {\n"
               "    \t\"city\" : \"Moscow\",\n"
               "        \"street\" : \"Vozdvijenka\"\n"
               "    }\n"
               "}");

    hello.parse();
    auto hi = any_cast<Json>(hello["marks"]);
    if (!any_cast<bool>(hello._meanings[3]))
    {
        cout << any_cast<int>(hi[4]) << std::endl;
        cout << any_cast<int>(hello["age"]) << std::endl;
    }

    return 0;
}
