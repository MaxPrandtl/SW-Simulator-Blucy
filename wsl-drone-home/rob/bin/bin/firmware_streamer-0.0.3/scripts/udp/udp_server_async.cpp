#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::udp;

const short PORT = 12345;

class udp_server {
public:
    udp_server(boost::asio::io_service& io_service)
        : socket_(io_service, udp::endpoint(udp::v4(), PORT)) {

        std::cout << "Server is running on port " << PORT << "...\n";
        
        // Start receiving data asynchronously
        async_receive();
    }

private:
    void async_receive() {
        socket_.async_receive_from(
            boost::asio::buffer(data_), sender_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_received) {
                if (!ec) {
                    // Handle the received data
                    std::cout << "Received: " << std::string(data_, bytes_received)
                              << " from " << sender_endpoint_ << "\n";

                    // Send a response asynchronously
                    async_send(bytes_received);
                } else {
                    std::cerr << "Receive failed: " << ec.message() << "\n";
                }
                // Continue receiving the next message
                async_receive();
            });
    }

    void async_send(std::size_t bytes_received) {
        std::string response = "Message received";

        socket_.async_send_to(
            boost::asio::buffer(response), sender_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_sent) {
                if (ec) {
                    std::cerr << "Send failed: " << ec.message() << "\n";
                } else {
                    std::cout << "Sent " << bytes_sent << " bytes to " << sender_endpoint_ << "\n";
                }
            });
    }

    udp::socket socket_;
    udp::endpoint sender_endpoint_;
    char data_[1024];
};

int main() {
    try {
        boost::asio::io_service io_service;
        
        // Create the UDP server object, which will start receiving data
        udp_server server(io_service);
        
        // Run the io_service to process asynchronous operations
        io_service.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
