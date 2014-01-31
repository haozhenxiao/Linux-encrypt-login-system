Linux-encrypt-login-system
==========================
Normally, the program getty displays the text ”login:” when users log in on a text
terminal. This program also accepts a username and executes another program called
/bin/login with this username as an argument. The program /bin/login prints
”Password:” and checks if the entered password is in fact the correct password for the
actual user. If it is correct, a command interpreter is executed. However, in our case,
the most practical approach is to let the login-program handle both the identification
(username) and the authentication (password) that the user supplies.
