#pragma once

#include <string>


class Caroline
{

private:

    std::string model;
    std::string personality;


public:

    void initialize(
        std::string modelPath,
        std::string personalityName
    );


    void status();


    std::string respond(
        std::string input
    );

};

