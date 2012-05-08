function []=sendEmailThroughGmail(to,subject,message)

user = char(java.lang.System.getProperty('user.name'));
hostname = char(getHostName(java.net.InetAddress.getLocalHost));

user = strcat(user,'@');
hostname = strcat(user,hostname);
subject = strcat(subject, ' at: ');
subject = strcat(subject,hostname);

setpref('Internet', 'E_mail', 'noisybsscpp@gmail.com');
setpref('Internet', 'SMTP_Username', 'noisybsscpp@gmail.com');
setpref('Internet', 'SMTP_Password', 'estrela1234');
setpref('Internet', 'SMTP_Server', 'smtp.gmail.com');
props = java.lang.System.getProperties;
props.setProperty('mail.smtp.auth','true');
props.setProperty('mail.smtp.socketFactory.class', 'javax.net.ssl.SSLSocketFactory');
props.setProperty('mail.smtp.socketFactory.port', '465');

sendmail(to,subject,message);
