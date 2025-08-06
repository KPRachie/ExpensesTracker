#include "command.hpp"

ICommand::ICommand(TgBot::Bot& bot) : m_bot(bot) {}

ICommand::~ICommand() {}
