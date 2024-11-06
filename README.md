# TCP/UDP Client server Banking Application with Real Time Notification

## Project Overview:
This project is a mock banking application built using Python, which leverages both TCP and UDP protocols to facilitate client-server communication. The main functionalities provided include user registration, login, balance inquiry, fund transfer, and account deletion. Additionally, it features real-time notifications, allowing users to receive instant alerts on transactions and other account activities.

## Key Features:

### 1.User Authentication:

    Users can sign up by providing a username, password, and mobile number.
    Login functionality allows users to authenticate using their credentials.
    
### 2. Banking Transactions:

    **Balance Inquiry:** Authenticated users can view their account balance.
    **Fund Transfer:** Users can transfer funds to other accounts within tthe system.
    **Account Deletion:** Users can delete their account after providing valid credentials.
    
### 3. Real-Time Notifications:

    Uses UDP to send immediate notifications to clients, such as successful fund transfers or account deletion alerts.
    Keeps users informed of important activities in real-time.
    
### 4. Concurrent Client Handling:

    The server supports multiple clients simultaneously using threading to manage each connection independently.
    Addresses of active clients are stored, enabling targeted notifications.

## Code Structure and Key Components:

### 1. Server-Side Code:

   **Server Configuration:** The server is set up to use a specified IP and port for TCP and UDP connections.
    
    **Client Handling:** Manages user authentication, transaction processing, and notifications within the handle_client function. This function handles different commands (e.g., SIGNUP, LOGIN, VIEW_DETAILS) sent by clients.
    
    **Real-Time Notification:** The send_udp_notification function sends notifications to active users using UDP.
    
  ** Database: **A mock dictionary-based database stores user credentials and account balances.

### 2. Client-Side Code:

    **Initial Menu:** Clients choose between Sign Up, Login, or Exit.
    
    **Main Menu**: Once authenticated, users access options like View Account Details, Transfer Funds, Check Balance, Delete Account, or Exit.
    
    **UDP Listener:** A separate thread runs to constantly listen for notifications sent from the server.
    
    **Notification Handler:** When a notification is received, it displays the message to the user and pauses until acknowledged.


### Workflow:

1.The server listens for TCP connections from clients.

2.Upon connection, the client sends commands like SIGNUP, LOGIN, etc., which the server processes and responds to.

3.For fund transfers and account deletions, the server sends UDP notifications to active clients to inform them of any relevant changes.

4.Each client has a dedicated thread to handle continuous UDP listening for real-time notifications, ensuring a smooth, responsive user experience.

### Conclution:
This project provides a simple yet effective demonstration of client-server communication, emphasizing real-time notifications in a banking context. It is an excellent learning model for applications that need simultaneous communication, client management, and UDP-based event notifications.

### Project Requirements:

_Operating System:_ Ubuntu (for both server and client virtual machines).
_Networking Protocols:_ TCP for secure, reliable communication; UDP for faster, non-critical messaging.
_Programming Language:_ Python for socket programming.
_Environment: _VirtualBox to simulate two machines (one client, one server).
_Security: _Basic encryption for secure transactions.

### Project Modules:

**Client Module (Bank User Interface):**
_Functionality: _Allows users to log in, view account details, and perform transactions.
_Network Type:_ TCP connection for reliable, secure communication.
_Connections: _1 TCP connection per session for secure data transfer.

**Server Module (Bank Central System):**
_Functionality: _Handles client requests ,and stores account details.
_Network Type:_ TCP for client requests, UDP for notifications.
_Connections: _Multiple TCP connections to handle concurrent clients; 1 UDP connection for real-time notifications.

**Transaction Processing Module:**
_Functionality:_ Processes various client transactions such as balance checks and fund transfers.
_Network Type: _TCP to ensure transactional data integrity.
_Connections:_ Shared TCP connection with client-server communication.

**Security Module:**
_Functionality:_ Ensures secure communication by validating client credentials before processing requests.
_Network Type:_ TCP to maintain a secure channel.
_Connections:_ 1 secure TCP connection per client login session.

**Notification/Alert Module:**
_Functionality: _Sends real-time alerts (e.g., transaction confirmations, suspicious activity) to clients.
_Network Type: _UDP for faster, less reliable delivery.
_Connections:_ 1 UDP connection for broadcasting alerts to clients.




    
