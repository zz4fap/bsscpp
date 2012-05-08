

for counter=5:1:10
cmd = sprintf('java -jar bsscpp_dbaccess.jar server1 insert_new_simulations simulations /Users/zz4fap/Desktop/finished_simulations_NEW/ SimulationsInformationForRecordings_%d.txt',counter);
[status, result] = system(cmd)
end

for counter=3:1:10
cmd = sprintf('java -jar bsscpp_dbaccess.jar server2 insert_new_simulations simulations /Users/zz4fap/Desktop/finished_simulations_NEW/ SimulationsInformationForRecordings_%d.txt',counter);
[status, result] = system(cmd)
end


%java -jar bsscpp_dbaccess.jar server1 insert_new_simulations simulations /Users/zz4fap/Desktop/finished_simulations_NEW/ SimulationsInformationForRecordings_5.txt;
%java -jar bsscpp_dbaccess.jar server1 insert_new_simulations simulations /Users/zz4fap/Desktop/finished_simulations_NEW/ SimulationsInformationForRecordings_5.txt;