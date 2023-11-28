import socket
from datetime import datetime

def handle_client(client_socket):
    # Get the current time
    current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    
    # Send the current time to the client
    client_socket.send(current_time.encode())
    
    # Close the client socket
    client_socket.close()

def simple_server():
    # Set the server address and port
    server_address = ('127.0.0.1', 8080)

    # Create a TCP socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Bind the socket to the server address and port
    server_socket.bind(server_address)

    # Listen for incoming connections (max 1 connection in the queue)
    server_socket.listen(1)
    print(f"Server listening on {server_address}")

    try:
        while True:
            # Wait for a connection
            print("Waiting for a connection...")
            client_socket, client_address = server_socket.accept()
            print(f"Accepted connection from {client_address}")

            # Handle the client's request
            handle_client(client_socket)
    except KeyboardInterrupt:
        print("\nServer shutting down...")
    finally:
        # Close the server socket
        server_socket.close()

if __name__ == "__main__":
    simple_server()
