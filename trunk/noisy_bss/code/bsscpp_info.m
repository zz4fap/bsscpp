function [status, result]=bsscpp_info(message)

hostname = char(getHostName(java.net.InetAddress.getLocalHost));

consumer_key= 'ywgdKN03dIOeFcbCKqAt9A';
consumer_secret= 'YqfGIdfz98GIy2ZBXtKJZkqrTDW1bN6kNb7ud5LAkTU';
oauth_token = '554373481-leeJCsbySBEgswdJTTg7ji45aasJuhMlpQEBJCxn';
oauth_token_secret = 'PHogD6Ng2J9qSta4RVe4RFjoxoaaRluPNmIBOsmgD3k';

hostname = strcat(hostname,': ');
message = strcat(hostname,message);

disp(message);

[status, result] = twit(message, consumer_key,consumer_secret,oauth_token,oauth_token_secret);
