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
    
   **Database**: A mock dictionary-based database stores user credentials and account balances.

    
