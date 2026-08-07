#include "model.hpp"
#include <iostream>

std::string ModelCommand::name()
{
    return "/model";
}

std::string ModelCommand::description()
{
    return "show intelligence model information";
}

void ModelCommand::execute()
{
    std::cout
    << "\nINTELLIGENCE MODEL\n\n"
    << "Name........Caroline\n"
    << "Backend......llama.cpp\n"
    << "Model........qwen2.5-1.5b-instruct-q4_k_m.gguf\n"
    << "Status.......NOT LOADED\n"
    << "Adapter......GLaDOS LoRA (NOT LOADED)\n\n";
}
