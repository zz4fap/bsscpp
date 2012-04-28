package com.bsscpp.jdbctests;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.sql.*;
import java.util.ArrayList;
import java.util.List;

import javax.management.InvalidAttributeValueException;

import com.bsscpp.jdbctests.Constants.INFO;

public class JdbcTest {

	private Constants.CMD mCmd;
	private SqlServer mServer;

	public static void main(String[] args) {

		JdbcTest jdbcTest = new JdbcTest();

		if(!jdbcTest.handleInputArgs(args)) {
			return;
		}
		
		jdbcTest.executeCommand();
	}
	
	private void executeCommand() {
		try {
			mServer.connectToDb();
			System.out.println("Connected to the database");

			switch(mCmd) {
			case POPULATE:
				populateSimulationsTable();
				break;
			case UPDATE:
				int numberOfUpdatedRows = updateSimulationsTable();
				System.out.println("Number of updated rows: "+numberOfUpdatedRows);
				break;
			case SELECT:
				selectAndShowFromSimulationsTable("*", null);
				break;
			case LOCK:
				lockTable("tests_table");
				break;
			case TEST_CONN:
				testConnection();
				break;
			case RAW:
			case UNKNOWN:
			default:
				System.out.println("Unknown command");
				return;
			}

			mServer.disconnectFromDb();
			System.out.println("Disconnected from database");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void testConnection() {
		System.out.println("URL: "+mServer.URL);
	}

	private boolean handleInputArgs(String[] args) {
		if(args.length > 1) {
			// check server.
			String str = (args[0]).toLowerCase();
			SqlServer.SERVER server;
			if("server1".equals(str)) {
				server = SqlServer.SERVER.SERVER1;
			} else if("server2".equals(str)) {
				server = SqlServer.SERVER.SERVER2;
			} else {
				System.out.println("Invalid server. \nAvailable Servers: "+SqlServer.SERVER.values());
				return false;		
			}
			mServer = SqlServer.getServer(server);
			// Check command.
			str = (args[1]).toLowerCase();
			if("populate".equals(str)) {
				mCmd = Constants.CMD.POPULATE;
			} else if("update".equals(str)) {
				mCmd = Constants.CMD.UPDATE;
			} else if("select".equals(str)) {
				mCmd = Constants.CMD.SELECT;
			} else if("lock".equals(str)) {
				mCmd = Constants.CMD.LOCK;
			} else if("test".equals(str)) {
				mCmd = Constants.CMD.TEST_CONN;
			} else {
				System.out.println("Invalid command. \nAvailable Commands: "+Constants.CMD.values());
				return false;
			}
			System.out.println("Command: "+ mCmd.toString());
		} else {
			System.out.println(Constants.HELP_MSG);
			return false;
		}
		return true;
	}

	private int updateSimulationsTable() {
		int[] info = null;
		int numberOfUpdatedRows = 0;
		Connection conn = mServer.getConnection();
		try {
			for(int i=1; i<=10; i++) {
				info = getLasRunSimulation(INFO.COMPLETED_SIMULATIONS_PATH+INFO.SIMULATION_INFO_FILE+i+".txt");

				if(info!=null){
					for(int j=1; j<=info[0]; j++) { //iterates over the set of wave files.
						for(int k=1; k<=24; k++) {

							int simulation_number = ((i-1)*2880 + ((j-1)*24 + k));

							numberOfUpdatedRows++;

							updateTable(conn, SqlServer.SIMULATIONS_TABLE_NAME, "status=2", "simulation_number="+simulation_number);

							System.out.println("counter: "+simulation_number+" - file: "+i+" - wav: "+j+" - N/L: "+k);

							if(j==info[0] && k==info[1]){
								break;
							} 
						}
					}
				} else {
					continue;
				}
				System.out.println("--------------");
			}
		} catch (InvalidAttributeValueException e) {
			e.printStackTrace();
		}
		return numberOfUpdatedRows;
	}

	//null if file exists and is empty or if it doesn't exist.
	//string vector with wave file number and simulation if there is some input.
	private int[] getLasRunSimulation(String filename) throws InvalidAttributeValueException {
		String str, lastRunSimulationString = null;
		int[] lastRunSimulationInfo = null;

		try {
			FileReader fileReader = new FileReader(filename);
			BufferedReader br = new BufferedReader(fileReader);

			while ((str = br.readLine()) != null) {
				lastRunSimulationString = str;
			}

			if(lastRunSimulationString!=null) {
				int i = 0;
				lastRunSimulationInfo = new int[2];
				for(String info : lastRunSimulationString.split(" ")) {
					lastRunSimulationInfo[i] = Integer.parseInt(info);
					i++;
				}
			}

		} catch (IOException e) {
			// Do nothing.
		}
		return lastRunSimulationInfo;
	}

	private void populateSimulationsTable() {
		int counter = 1;
		List<String> recordings;
		Connection conn = mServer.getConnection();

		for(int i=1; i<=10; i++) {
			recordings = openRecordingsFile(INFO.RECORDINGS_FILENAME + i);
			for(String recording : recordings) {

				for(double stepsize : INFO.STEPSIZE) {

					for(int n=0; n<INFO.N.length; n++) {

						insertIntoSimulationsTable(conn, recording, INFO.NOISE_FILENAME, stepsize, INFO.N[n], INFO.L[n], INFO.LEARNING_METHOD, INFO.EPOCHS);

						System.out.println("counter: " + counter
								+ " wav_filename: " + recording
								+ " noise_filename: " + INFO.NOISE_FILENAME
								+ " stepsize: " + stepsize
								+ " N: " + INFO.N[n]
										+ " L: " + INFO.L[n]
												+ " learning_method: " + INFO.LEARNING_METHOD
												+ " epochs: " + INFO.EPOCHS);

						counter++;
					}
				}
			}
		}
	}

	private List<String> openRecordingsFile(String filename) {
		List<String> recordings = new ArrayList<String>();

		try {
			// Open the file that is the first
			// command line parameter
			FileInputStream fstream = new FileInputStream(INFO.RECORDINGS_PATH + filename);
			// Get the object of DataInputStream
			DataInputStream in = new DataInputStream(fstream);
			BufferedReader br = new BufferedReader(new InputStreamReader(in));
			String recording;
			// Read File Line By Line
			while ((recording = br.readLine()) != null) {
				recordings.add(recording);
			}
			// Close the input stream
			in.close();
		} catch (Exception e) {// Catch exception if any
			System.err.println("Error: " + e.getMessage());
		}

		return recordings;
	}

	private void insertIntoSimulationsTable(Connection conn, String wav_filename, String noise_filename, double stepsize, int N, int L, String learning_method, int epochs) {
		try {
			Statement st = conn.createStatement();
			int count = st.executeUpdate("INSERT INTO simulations (wav_filename, noise_filename, stepsize, N, L, learning_method, epochs) VALUES ('"+wav_filename+"', '"+noise_filename+"', "+stepsize+", "+N+", "+L+",'"+learning_method+"', "+epochs+")");
			System.out.println(count + " rows were inserted");

		} catch (SQLException e) {
			System.out.println("SQL code does not execute.");
			System.err.println("Error message: " + e.getMessage());
			System.err.println("Error number: " + e.getErrorCode());
		}
	}

	private void selectAndShowFromSimulationsTable(String what, String where_clause) {
		Connection conn = mServer.getConnection();
		ResultSet res = selectFromTable(conn, SqlServer.SIMULATIONS_TABLE_NAME, what, where_clause);
		try {
			while (res.next()) {
				System.out.println(res.toString());
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}

	private ResultSet selectFromTable(Connection conn, String table, String what, String where_clause) {
		ResultSet res = null;
		try {
			Statement st = conn.createStatement();
			if(where_clause!=null) {
				res = st.executeQuery("SELECT "+what+" FROM  "+table+" WHERE "+where_clause+"");
			} else {
				res = st.executeQuery("SELECT "+what+" FROM  "+table+"");
			}
		} catch (SQLException e) {
			System.out.println("SQL code does not execute.");
			System.err.println("Error message: " + e.getMessage());
			System.err.println("Error number: " + e.getErrorCode());
		}		
		return res;
	}

	private void updateTable(Connection conn, String table, String set_clause, String where_clause) {
		try {
			Statement st = conn.createStatement();
			int count = 0;
			if(where_clause!=null){
				count = st.executeUpdate("UPDATE "+table+" SET "+set_clause+" WHERE "+where_clause+"");
			} else {
				st.executeUpdate("UPDATE "+table+" SET "+set_clause+"");
			}
			System.out.println(count + " rows were updated");
		} catch (SQLException e) {
			System.out.println("SQL code does not execute.");
			System.err.println("Error message: " + e.getMessage());
			System.err.println("Error number: " + e.getErrorCode());
		}
	}

	private void lockTable(String table) {
		Connection conn = mServer.getConnection();
		String lock = "LOCK TABLE " + table + " WRITE";
		Statement stmt = null;
		try {
			conn.setAutoCommit(false);
			stmt = conn.createStatement();
			stmt.execute(lock);
			System.out.println("Locking now....");
			try {
				for(int i=0; i<4; i++) {
					Thread.sleep(9000);
					stmt.executeQuery("SELECT * FROM  "+table+"");
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			//int count = stmt.executeUpdate("INSERT INTO "+table+" (wav_filename, noise_filename, stepsize, N, L, learning_method, epochs) VALUES ('arroba.wav', 'babble.wav', 0.023, 256, 16,'fixedstepsize', 500)");
			//System.out.println(count + " rows were inserted");
			conn.commit();
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			if (stmt != null) {
				try {
					stmt.execute("UNLOCK TABLES");
				} catch (SQLException e) {
					e.printStackTrace();
				}
			}
		}
	}
}
