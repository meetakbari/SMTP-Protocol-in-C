# Author Details

* **Meet Akbari**  
* **Mayankkumar Tank**
* **Course** : Computer Networks | University 5th semester project
* **Organization** : School of Engineering and Applied Science, Ahmedabad University, India.

## How to run client and server file:
* First compile the server.c file and run using this format : ./server portnumber(any range except 0-1023)
* Then compile the client.c file and run using this format : ./client 127.0.0.1 portnumber(same as written on server side)


## To communicate with server there are several commands(here are 6 which are implemented in this project):
1. HELO - to start conversation for sending email 
2. MAIL FROM - to send sender's email address
3. RCPT TO - to send recipient's email address
4. DATA - to send data which main content of the email (body part)
5. ATTACHMENT - to send an image file as an attachment with email
6. QUIT - to terminate the connection with server

#### NOTE : must follow the order of commands as given above while executing the client code.

* When you will run the client code, you will be prompted "Enter the command:" statement in which you have to write this commands(necessary to use all capital letters only) appropriately to make conversation with server. 

* For command number 2 to 5 you will be prompted another more statements to write the information like sender's emailid, recipient's emailid, subject of the email, content of the email etc. Write the information carefully to avoid ambiguity.

* For the attachment command, you will be asked to enter the file path or file name. If the file is not available in the same directory of client code then you must have to write the full path to the image file.

* NOTE : read the instructions and statements which are displayed on the console while you are running the client code using appropriate commands specially for the multiple lines content.

* Appropriate send and receive messages are displaying on both the side of client and server. So read it carefully and go ahead!

* We have given you one image as a reference for testing, and at the server side image will be by default saved with "received.jpeg" name. So, we recommand you to use .jpeg format only to avoid ambiguity at both the side.


I hope you will like our efforts to teach you how SMTP protocol works command by command when any client wants to send the email to SMTP server.

~ Thank you! 

## Contributors
[![](https://avatars1.githubusercontent.com/u/56075605?s=50&u=bf99d5c66a0749903135b279cf00e8ecf0e26d77&v=4)](https://github.com/meetakbari)
[![](https://avatars0.githubusercontent.com/u/55320599?s=50&v=4)](https://github.com/MayankkumarTank)
