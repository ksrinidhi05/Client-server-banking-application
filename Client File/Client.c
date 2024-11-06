import socket
import threading

# TCP server IP and Port
SERVER_IP = '192.168.56.1'  # Replace with your server's IP
SERVER_PORT = 12345

# UDP notification server IP and port
UDP_IP = '192.168.56.1'  # Replace with your local UDP server IP
UDP_PORT = 54321

notification_pending = False

def display_initial_menu():
    """Display the initial menu to choose Sign Up or Login."""
    print("\n---- Initial Menu ----")
    print("1. Sign Up")
    print("2. Login")
    print("3. Exit")

def display_menu():
    """Display the main menu after successful login."""
    print("\n---- Menu ----")
    print("1. View Account Details")
    print("2. Transfer Funds")
    print("3. Check Balance")
    print("4. Delete Account")
    print("5. Exit")

def udp_listen():
    """Continuously listen for UDP notifications from the server in a separate thread."""
    udp_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_sock.bind(('', UDP_PORT))
    
    global notification_pending
    try:
        while True:
            data, addr = udp_sock.recvfrom(1024)
            if data:
                notification = data.decode()
                notification_pending = True
                handle_notification(notification)
                notification_pending = False
    except Exception as e:
        print(f"Error in UDP listening: {e}")
    finally:
        udp_sock.close()

def handle_notification(notification):
    """Display the notification, prompt the user to continue, and show the menu."""
    print("\n--- Notification ---")
    print(notification)
    print("--------------------")
    print("Press Enter to continue...")
    input()  # Wait for the user to press Enter
    display_menu()  # Show the menu after the notification

def main():
    # Create a TCP socket for client-server communication
    client_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_sock.connect((SERVER_IP, SERVER_PORT))
    
    # Start listening for UDP notifications in the background
    threading.Thread(target=udp_listen, daemon=True).start()
    
    authenticated = False
    
    try:
        while True:
            # Show the initial menu until the user successfully logs in
            if not authenticated:
                display_initial_menu()
                initial_choice = input("Enter choice: ")
                
                if initial_choice == '1':  # Sign Up
                    # Collect sign-up information
                    username = input("Enter new username: ")
                    password = input("Enter new password: ")
                    verify_password = input("Re-enter password to verify: ")
                    mobile_number = input("Enter your mobile number: ")

                    # Check if passwords match
                    if password != verify_password:
                        print("Passwords do not match. Please try signing up again.")
                        continue
                    
                    # Send the sign-up data to the server
                    client_sock.send(f"SIGNUP {username} {password} {mobile_number}".encode())
                    response = client_sock.recv(1024).decode()
                    print("Server Response:", response)
                    if response == "Sign Up successful":
                        print("Please log in with your new credentials.")
                    continue
                
                elif initial_choice == '2':  # Login
                    username = input("Enter username: ")
                    password = input("Enter password: ")
                    client_sock.send(f"LOGIN {username} {password}".encode())
                    response = client_sock.recv(1024).decode()
                    print("Server Response:", response)
                    if response == "Login successful":
                        authenticated = True
                        display_menu()  # Show the main menu after successful login
                    continue

                elif initial_choice == '3':  # Exit
                    print("Exiting...")
                    client_sock.send("EXIT".encode())
                    break
                else:
                    print("Invalid choice. Please try again.")
                    continue

            # Main menu after login
            choice = input("Enter choice: ")
            
            if choice == '1':  # View Account Details
                client_sock.send("VIEW_DETAILS".encode())
                response = client_sock.recv(1024).decode()
                print("Account Details:", response)
            
            elif choice == '2':  # Transfer Funds
                amount = input("Enter amount to transfer: ")
                recipient_username = input("Enter recipient's username: ")
                client_sock.send(f"TRANSFER {amount} {recipient_username}".encode())
                response = client_sock.recv(1024).decode()
                print("Transfer Response:", response)
                
                # Wait for notifications if any are displayed
                print("\nPress Enter to continue...")
                input()  # Wait for the user to acknowledge the notification
            
            elif choice == '3':  # Check Balance
                client_sock.send("CHECK_BALANCE".encode())
                response = client_sock.recv(1024).decode()
                print("Balance:", response)

            elif choice == '4':  # Delete Account
                del_username = input("Enter username to delete: ")
                del_password = input("Enter password for deletion: ")
                client_sock.send(f"DELETE_ACCOUNT {del_username} {del_password}".encode())
                response = client_sock.recv(1024).decode()
                print("Deletion Response:", response)

                # Wait for notifications if any are displayed
                print("\nPress Enter to continue...")
                input()  # Wait for the user to acknowledge the notification
            
            elif choice == '5':  # Exit
                print("Exiting...")
                client_sock.send("EXIT".encode())
                break

            # Display the menu again after processing the user's choice
            display_menu()

    except KeyboardInterrupt:
        print("\nClient shutting down...")

    finally:
        client_sock.close()
        print("Client connection closed.")

if name == "main":
    main()

import socket
import threading

# Server configuration
SERVER_IP = '192.168.56.1'  # Replace with your server's IP
SERVER_PORT = 12345
UDP_PORT = 54321

# Mock database for user details and balances
users = {"user1": "pass1", "user2": "pass2"}
accounts = {"user1": 1000, "user2": 2000}

# To store active client addresses for notifications
client_addresses = {}

def handle_client(client_sock, addr):
    print(f"Connection from {addr}")
    username = None
    authenticated = False
    
    while True:
        try:
            data = client_sock.recv(1024).decode()
            
            if not data:
                break
            
            if data.startswith("SIGNUP"):
                # Handle new user registration
                _, new_username, password, mobile_number = data.split()
                if new_username in users:
                    client_sock.send("Username already exists".encode())
                else:
                    users[new_username] = password
                    accounts[new_username] = 0  # Initialize balance
                    client_sock.send("Sign Up successful".encode())
            
            elif data.startswith("LOGIN"):
                # Handle user login
                _, username, password = data.split()
                if username in users and users[username] == password:
                    authenticated = True
                    client_addresses[username] = addr  # Track client address
                    client_sock.send("Login successful".encode())
                else:
                    client_sock.send("Invalid credentials".encode())
            
            elif data == "VIEW_DETAILS":
                # Show account details if logged in
                if authenticated:
                    client_sock.send(f"Account Details: Username: {username}, Balance: ${accounts[username]}".encode())
                else:
                    client_sock.send("Please log in first.".encode())
            
            elif data.startswith("TRANSFER"):
                # Handle fund transfer if logged in
                if authenticated:
                    _, amount, recipient_username = data.split()
                    amount = float(amount)
                    if accounts[username] >= amount and recipient_username in users:
                        accounts[username] -= amount
                        accounts[recipient_username] += amount
                        client_sock.send(f"Transferred {amount} to {recipient_username}. New balance: {accounts[username]}".encode())
                        
                        # Send UDP notification to the recipient if online
                        if recipient_username in client_addresses:
                            recipient_addr = client_addresses[recipient_username]
                            notification = f"You received ${amount} from {username}"
                            send_udp_notification(recipient_addr, notification)

                        # Server notification
                        print(f"Transaction Notification: {username} transferred ${amount} to {recipient_username}.")
                        print(f"Updated Balances - {username}: ${accounts[username]}, {recipient_username}: ${accounts[recipient_username]}")
                    else:
                        client_sock.send("Insufficient balance or recipient not found.".encode())
                else:
                    client_sock.send("Please log in first.".encode())
            
            elif data == "CHECK_BALANCE":
                # Send balance information if logged in
                if authenticated:
                    client_sock.send(f"Balance: ${accounts[username]}".encode())
                else:
                    client_sock.send("Please log in first.".encode())

            elif data.startswith("DELETE_ACCOUNT"):
                # Handle account deletion
                _, del_username, del_password = data.split()
                if del_username in users and users[del_username] == del_password:
                    del users[del_username]
                    del accounts[del_username]
                    client_sock.send("Account deleted successfully.".encode())
                    print(f"Account Deleted: {del_username}")  # Server side notification
                    # Notify other clients
                    for client in client_addresses:
                        if client != del_username:
                            notification = f"Account {del_username} has been deleted."
                            send_udp_notification(client_addresses[client], notification)
                else:
                    client_sock.send("Invalid username or password. Account deletion failed.".encode())

            elif data == "EXIT":
                print(f"Client {addr} disconnected.")
                break
        
        except Exception as e:
            print(f"Error handling client {addr}: {e}")
            break
    
    # Clean up on client disconnect
    if username:
        client_addresses.pop(username, None)
    client_sock.close()
    print(f"Server connection closed for {addr}.")

def send_udp_notification(client_addr, message):
    """Send a real-time notification via UDP to the specified client address."""
    udp_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_sock.sendto(message.encode(), (client_addr[0], UDP_PORT))
    udp_sock.close()

def start_server():
    # Set up the TCP server
    server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_sock.bind((SERVER_IP, SERVER_PORT))
    server_sock.listen(5)
    print(f"Server listening on {SERVER_IP}:{SERVER_PORT}")
    
    while True:
        client_sock, addr = server_sock.accept()
        client_thread = threading.Thread(target=handle_client, args=(client_sock, addr))
        client_thread.start()

if name == "main":
    start_server()
for these two code give a proper title and also give some insights about it
