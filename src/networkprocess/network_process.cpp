#include "network_process.h"
#include <asio.hpp>
#include <thread>
#include <unistd.h>
#include "communicator.h"
#include "logger_instance.h"

namespace process
{
    void NetworkProcess::work()
    {
        preWork();
        tools::LoggerManager::createLoggerType();
        // Real process work implementation
        Communicator::getInstance().sendCreationMessage();
        tools::LoggerManager::getInstance() << "[PROCESS EXECUTING] | Process work started";
        tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
        // Add real process work code here

        try
        {
            asio::io_context io_context;

            // Resolve the host
            asio::ip::tcp::resolver               resolver(io_context);
            asio::ip::tcp::resolver::results_type endpoints = resolver.resolve("www.example.com", "80");

            // Create a socket and connect to the host
            asio::ip::tcp::socket socket(io_context);
            asio::connect(socket, endpoints);

            // Form the HTTP request
            std::string request = "GET / HTTP/1.1\r\n";
            request += "Host: www.example.com\r\n";
            request += "Connection: close\r\n\r\n";

            tools::LoggerManager::getInstance() << "[PROCESS EXECUTING] | Sending request";
            tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
            // Send the request
            asio::write(socket, asio::buffer(request));

            // Read the response
            asio::streambuf response;
            asio::read_until(socket, response, "\r\n");

            // Parse the HTTP response status line
            std::istream response_stream(&response);
            std::string  http_version;
            response_stream >> http_version;
            unsigned int status_code;
            response_stream >> status_code;
            std::string status_message;
            std::getline(response_stream, status_message);

            if (!response_stream || http_version.substr(0, 5) != "HTTP/")
            {
                tools::LoggerManager::getInstance() << "[PROCESS EXECUTING] | Invalid response";
                tools::LoggerManager::getInstance().flush(tools::LogLevel::ERROR);
                return;
            }
            if (status_code != 200)
            {
                tools::LoggerManager::getInstance()
                        << "[PROCESS EXECUTING] | Response returned with status code " << status_code;
                tools::LoggerManager::getInstance().flush(tools::LogLevel::ERROR);
                return;
            }

            // Read the headers
            asio::read_until(socket, response, "\r\n\r\n");
            std::string header;
            tools::LoggerManager::getInstance()
                    << "[PROCESS EXECUTING] | Response returned with status code " << status_code;
            tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
            while (std::getline(response_stream, header) && header != "\r")
            {
                tools::LoggerManager::getInstance() << header;
                tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
            }
            tools::LoggerManager::getInstance() << "[PROCESS EXECUTING] | After response_stream\n";
            tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);

            // Write the remaining response to the output
            if (response.size() > 0)
            {
                std::istream remaining_stream(&response);
                std::string  remaining_content((std::istreambuf_iterator<char>(remaining_stream)),
                                               std::istreambuf_iterator<char>());
                tools::LoggerManager::getInstance() << remaining_content;
                tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
            }

            // Read until EOF
            asio::error_code error;
            while (asio::read(socket, response, asio::transfer_at_least(1), error))
            {
                std::istream response_stream(&response);
                std::string  content((std::istreambuf_iterator<char>(response_stream)),
                                     std::istreambuf_iterator<char>());
                tools::LoggerManager::getInstance() << content;
                tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
            }
            if (error != asio::error::eof)
            {
                throw asio::system_error(error);
            }
            tools::LoggerManager::getInstance() << "[PROCESS EXECUTING] | Response received. Ending...";
            tools::LoggerManager::getInstance().flush(tools::LogLevel::INFO);
        }
        catch (const std::exception &e)
        {
            tools::LoggerManager::getInstance() << "[PROCESS EXECUTING] | Exception: " << e.what();
            tools::LoggerManager::getInstance().flush(tools::LogLevel::EXCEPTION);
        }
        postWork();
        logLifetime();
        _exit(exitCode_); // Ensure the child process exits immediately
    }

} // namespace process
