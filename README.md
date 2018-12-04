# silverfish

Web interface for controlling a robot

# contributors

Alex Brooke brookea@purdue.edu

# technology

This project uses Amazon Web Services in a few ways:

- An AWS bucket hosts the webpage responsible for providing users with control of the robot
  - The webpage is written in HTML and JavaScript
  - Users must log in using a Google account before interacting with the robot
  - The user interface has a few buttons representing actions the robot will be able to take
    - Each button sends an AWS SNS message
- An AWS DynamoDB instance logs the actions users have sent to the robot
- AWS Cognito will handle user authentication
- An AWS Lambda acts as the "server" for the interface, sending relevant data to the robot
  - The Lambda function is triggered by the SNS messages sent from the frontend
  - During execution, the function parses the instruction received from the frontend, and publishes a translated instruction to the robot
- The robot is realized as an AWS IoT Thing that subscribes to the topic that the Lambda function publishes to

![](silverfish.png)
