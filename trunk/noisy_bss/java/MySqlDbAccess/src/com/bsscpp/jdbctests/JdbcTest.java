package com.bsscpp.jdbctests;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import javax.management.InvalidAttributeValueException;

import com.bsscpp.jdbctests.Constants.*;

public class JdbcTest {

	private Constants.CMD mCmd;
	private SqlServer mServer;
	private String mTable;
	private String mWhat;
	private String mWhere;
	private int mNewStatus;
	private int mSimulationNumber;
	private String mNewResultsFile;
	private String mPath2NewResults;

	public static void main(String[] args) {

		JdbcTest jdbcTest = new JdbcTest();

		if(!jdbcTest.handleInputArgs(args)) {
			return;
		}

		int res = jdbcTest.executeCommand();
		System.out.println("Command result: "+res);
		System.exit(res);
	}

	private int executeCommand() {
		int result = RET_VALUES.GENERIC_ERROR;
		try {
			if(!mServer.connectToDb()) {
				System.out.println("It was impossible to connect to the SQL server... aborting operation...");
				return RET_VALUES.SERVER_IS_DOWN_ERROR;
			}
			System.out.println("Connected to the database");

			switch(mCmd) {
			case POPULATE:
				result = populateSimulationsTable();
				break;
			case UPDATE:
				result = updateSimulationsTable();
				System.out.println("Number of updated rows: "+result);
				break;
			case SELECT:
				result = selectAndShowFromSimulationsTable(SqlServer.SIMULATIONS_TABLE_NAME, "*", "status=2");
				break;
			case LOCK:
				result = lockTable("tests_table", 9000);
				break;
			case TEST_CONN:
				result = testConnection();
				break;
			case RETRIEVE:
				result = retrieveAvailableSimulations(mTable, mWhat, mWhere, mNewStatus);
				System.out.println("Number of available simulations: "+result);
				break;
			case UPDATE_STATUS:
				result = updateStatus(mTable, mNewStatus, mSimulationNumber);
				break;
			case UPDATE_STATUS_OF_FILE:
				result = updateStatusOfSimulationsInFile(mTable, mNewStatus, INFO.AVAILABLE_SIMULATIONS_FILE);
				break;
			case INSERT_NEW_FINISHED_SIMULATONS:
				result = updateDataBaseWithNewRunSimulations(mTable, mPath2NewResults, mNewResultsFile);
				break;
			case ADD_RESULTS:
				result = addResultsToResultsTable();
				break;
			case RAW:
			case UNKNOWN:
			default:
				System.out.println("Unknown command");
				return result;
			}

			mServer.disconnectFromDb();
			System.out.println("Disconnected from database");
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}

	public int testConnection() {
		System.out.println("URL: "+mServer.URL);
		return RET_VALUES.SUCCESS;
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
			} else if("retrieve".equals(str)) {
				mCmd = Constants.CMD.RETRIEVE;
				if(!handleRetrieveCmdArgs(args)) {
					return false;
				}
			} else if("update_status".equals(str)) {
				mCmd = Constants.CMD.UPDATE_STATUS;
				if(!handleUpdateStatusCmdArgs(args)) {
					return false;
				}
			} else if("update_status_of_file".equals(str)) {
				mCmd = Constants.CMD.UPDATE_STATUS_OF_FILE;
				if(!handleUpdateStatusOfFileCmdArgs(args)) {
					return false;
				}
			} else if("insert_new_simulations".equals(str)) {
				mCmd = Constants.CMD.INSERT_NEW_FINISHED_SIMULATONS;
				if(!handleInsertNewSimulationsCmdArgs(args)) {
					return false;
				}
			} else if("add_results".equals(str)) {
				mCmd = Constants.CMD.ADD_RESULTS;
			} else {
				System.out.println("Invalid command. \nAvailable Commands: \n"+Constants.CMD.getCmds());
				return false;
			}
			System.out.println("Command: "+ mCmd.toString());
		} else {
			System.out.println(Constants.HELP_MSG);
			return false;
		}
		return true;
	}

	public boolean handleInsertNewSimulationsCmdArgs(String[] args) {
		if(args.length < 5) {
			System.out.println("Usage: insert_new_simulations [table] [path_2_folder_with_new_results] [file_with_new_results]");
			return false;
		} else {
			// figure out which table must be used.
			mTable = null;
			for(String table: SqlServer.TABLES) {
				if(table.equals(args[2])) {
					mTable = table;
					break;
				}
			}
			if(mTable==null) {
				System.out.println("Invalid table name: "+args[2]);
				return false;
			}

			if("".equals(args[3])) {
				return false;
			}			
			mPath2NewResults = args[3];

			if("".equals(args[4])) {
				return false;
			}
			mNewResultsFile = args[4];
			System.out.println("insert_new_simulations "+mTable+" "+mNewResultsFile);
			return true;
		}
	}

	public boolean handleRetrieveCmdArgs(String[] args) {
		if(args.length < 6) {
			System.out.println("retrieve [table] [what] [where] [new_status]");
			return false;
		} else {
			// figure out which table must be used.
			mTable = null;
			for(String table: SqlServer.TABLES) {
				if(table.equals(args[2])) {
					mTable = table;
					break;
				}
			}
			if(mTable==null) {
				System.out.println("Invalid table name: "+args[2]);
				return false;
			}
			if("all_columns".equals(args[3])) {
				mWhat = "*";
			} else {
				mWhat = args[3];
			}
			mWhere = args[4];
			mNewStatus = Integer.parseInt(args[5]);
			System.out.println("retrieve "+mTable+" "+mWhat+" "+mWhere+" "+mNewStatus);
			return true;
		}
	}

	public boolean handleUpdateStatusCmdArgs(String[] args) {
		if(args.length < 5) {
			System.out.println("update_status [table] [new_status] [simulation_number]");
			return false;
		} else {
			// figure out which table must be used.
			mTable = null;
			for(String table: SqlServer.TABLES) {
				if(table.equals(args[2])) {
					mTable = table;
					break;
				}
			}
			if(mTable==null) {
				System.out.println("Invalid table name: "+args[2]);
				return false;
			}
			mNewStatus = Integer.parseInt(args[3]);
			mSimulationNumber = Integer.parseInt(args[4]);
			System.out.println("update_status "+mTable+" "+mNewStatus+" "+mSimulationNumber);
			return true;
		}
	}

	public boolean handleUpdateStatusOfFileCmdArgs(String[] args) {
		if(args.length < 4) {
			System.out.println("update_status_of_file [table] [new_status]");
			return false;
		} else {
			// figure out which table must be used.
			mTable = null;
			for(String table: SqlServer.TABLES) {
				if(table.equals(args[2])) {
					mTable = table;
					break;
				}
			}
			if(mTable==null) {
				System.out.println("Invalid table name: "+args[2]);
				return false;
			}
			mNewStatus = Integer.parseInt(args[3]);
			System.out.println("update_status "+mTable+" "+mNewStatus);
			return true;
		}
	}

	private int updateSimulationsTable() {
		int[] info = null;
		int numberOfUpdatedRows = 0;

		try {
			for(int i=1; i<=10; i++) {
				info = getLastRunSimulation(INFO.COMPLETED_SIMULATIONS_PATH+INFO.SIMULATION_INFO_FILE+i+".txt");

				if(info!=null) {
					for(int j=1; j<=info[0]; j++) { //iterates over the set of wave files.
						for(int k=1; k<=24; k++) {

							int simulation_number = ((i-1)*2880 + ((j-1)*24 + k));

							numberOfUpdatedRows++;

							updateTable(SqlServer.SIMULATIONS_TABLE_NAME, "status=2", "simulation_number="+simulation_number);

							String simulationInfo = "counter="+simulation_number+",file="+i+",wav="+j+",N/L="+k;
							System.out.println(simulationInfo);
							Utils.writeToFile(INFO.COMPLETED_SIMULATIONS_PATH+INFO.UPDATE_SIMULATION_INFO_FILE,simulationInfo+"\n",true);

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

	public int updateDataBaseWithNewRunSimulations(String table2Update, String path2NewResults, String newResultsFile) {
		int numberOfUpdatedRows = 0;
		String[] splitString = newResultsFile.split("_");
		splitString = (splitString[1]).split("\\.");
		int i = Integer.parseInt(splitString[0]);

		try {
			System.out.println("Old: "+INFO.COMPLETED_SIMULATIONS_PATH+INFO.SIMULATION_INFO_FILE+i+".txt");
			System.out.println("New: "+path2NewResults+newResultsFile);
			int[] oldInfo = getLastRunSimulation(INFO.COMPLETED_SIMULATIONS_PATH+INFO.SIMULATION_INFO_FILE+i+".txt");
			int[] newInfo = getLastRunSimulation(path2NewResults+newResultsFile);

			if(oldInfo[0]==newInfo[0] && oldInfo[1]==newInfo[1]) {
				System.out.println("Both files have the same number of simulations!!! Not updating...");
				return 0;
			}

			if(oldInfo!=null && newInfo!=null) {
				System.out.println("Old wav#: "+oldInfo[0]+", old simu#: "+oldInfo[1]);
				System.out.println("New wav#: "+newInfo[0]+", new simu#: "+newInfo[1]);
				for(int j=oldInfo[0]; j<=newInfo[0]; j++) { //iterates over the set of wave files.
					for(int k=(oldInfo[1]+1); k<=24; k++) {

						int simulation_number = ((i-1)*2880 + ((j-1)*24 + k));

						numberOfUpdatedRows++;

						updateTable(table2Update, "status=2", "simulation_number="+simulation_number);

						String simulationInfo = "counter="+simulation_number+",file="+i+",wav="+j+",N/L="+k;
						System.out.println(simulationInfo);

						if(j==newInfo[0] && k==newInfo[1]){
							break;
						} 
					}
					oldInfo[1] = 0;
				}
			} 
		} catch (InvalidAttributeValueException e) {
			e.printStackTrace();
		}
		return numberOfUpdatedRows;
	}

	//null if file exists and is empty or if it doesn't exist.
	//string vector with wave file number and simulation if there is some input.
	private int[] getLastRunSimulation(String filename) throws InvalidAttributeValueException {
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

	private int populateSimulationsTable() {
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

		return RET_VALUES.SUCCESS;
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

	private int selectAndShowFromSimulationsTable(String table, String what, String where_clause) {
		int result = RET_VALUES.SUCCESS;
		ResultSet res = selectFromTable(table, what, where_clause);
		try {
			while (res.next()) {
				System.out.println(res.toString());
			}
		} catch (SQLException e) {
			e.printStackTrace();
			result = RET_VALUES.GENERIC_ERROR;
		}

		return result;
	}

	private ResultSet selectFromTable(String table, String what, String where_clause) {
		ResultSet res = null;
		Connection conn = mServer.getConnection();
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

	private void updateTable(String table, String set_clause, String where_clause) {
		Connection conn = mServer.getConnection();
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

	private int lockTable(String table, long time) {
		int res = RET_VALUES.SUCCESS;
		Connection conn = mServer.getConnection();
		String lock = "LOCK TABLE " + table + " WRITE";
		Statement stmt = null;
		try {
			conn.setAutoCommit(false);
			stmt = conn.createStatement();
			stmt.execute(lock);
			System.out.println("Locking now....");
			for(int i=0; i<4; i++) {
				Thread.sleep(time);
			}
			conn.commit();
		} catch (Exception e) {
			e.printStackTrace();
			res = RET_VALUES.GENERIC_ERROR;
		} finally {
			if (stmt != null) {
				try {
					stmt.execute("UNLOCK TABLES");
					System.out.println("Table unlocked.");
				} catch (SQLException e) {
					e.printStackTrace();
					res = RET_VALUES.GENERIC_ERROR;
				}
			}
		}
		return res;
	}

	public int retrieveAvailableSimulations(String table, String what, String where_clause, int newStatus) {
		int numberOfAvailableSimulations = 0;
		ResultSet res = null;
		Statement stmt = null;
		Connection conn = mServer.getConnection();
		String lock = "LOCK TABLE " + table + " WRITE";
		try {
			conn.setAutoCommit(false);
			stmt = conn.createStatement();
			stmt.execute(lock);
			System.out.println("Locking now....");

			System.out.println("SELECT "+what+" FROM "+table+" WHERE "+where_clause+" LIMIT 5"+"");

			res = stmt.executeQuery("SELECT "+what+" FROM "+table+" WHERE "+where_clause+" LIMIT 5"+"");

			while (res.next()) {	
				int simulation_number = res.getInt("simulation_number");
				String wav = res.getString("wav_filename");
				String noise = res.getString("noise_filename");
				double stepsize = res.getDouble("stepsize");
				int N = res.getInt("N");
				int L = res.getInt("L");
				String learning_method = res.getString("learning_method");
				int epochs = res.getInt("epochs");
				int status = res.getInt("status");

				PreparedStatement preparedStatement = conn.prepareStatement("UPDATE "+table+" SET status=? " + "WHERE simulation_number=?");
				preparedStatement.setInt(1,newStatus); 
				preparedStatement.setInt(2,simulation_number); 
				preparedStatement.execute(); 
				preparedStatement.clearParameters();

				numberOfAvailableSimulations++;

				String result = "simulation_number=" + simulation_number
						+ ",wav_filename=" + wav + ",noise_filename=" + noise
						+ ",stepsize=" + stepsize + ",N=" + N + ",L=" + L
						+ ",learning_method=" + learning_method + ",epochs="
						+ epochs + ",status=" + status;
				Utils.writeToFile(INFO.AVAILABLE_SIMULATIONS_FILE, result+"\n",true);
				Utils.writeToFile(INFO.RETRIEVED_SIMULATIONS_FILE, result+"\n",true);
				System.out.println(result);
			}
			conn.commit();
		} catch (SQLException e) {
			e.printStackTrace();
			numberOfAvailableSimulations = RET_VALUES.GENERIC_ERROR;
		} finally {
			if (stmt != null) {
				try {
					stmt.execute("UNLOCK TABLES");
					System.out.println("Table unlocked.");
				} catch (SQLException e) {
					e.printStackTrace();
					numberOfAvailableSimulations = RET_VALUES.GENERIC_ERROR;
				}
			}
		}
		return numberOfAvailableSimulations;
	}

	public int updateStatus(String table, int newStatus, int simulation_number) {
		int ret = 1;
		Statement stmt = null;
		Connection conn = mServer.getConnection();
		String lock = "LOCK TABLE " + table + " WRITE";
		try {
			conn.setAutoCommit(false);
			stmt = conn.createStatement();
			stmt.execute(lock);
			System.out.println("Locking now....");

			PreparedStatement preparedStatement = conn.prepareStatement("UPDATE "+table+" SET status=? " + "WHERE simulation_number=?");
			preparedStatement.setInt(1,newStatus); 
			preparedStatement.setInt(2,simulation_number); 
			preparedStatement.execute(); 
			preparedStatement.clearParameters();

			conn.commit();
		} catch (SQLException e) {
			e.printStackTrace();
			ret = RET_VALUES.GENERIC_ERROR;
		} finally {
			if (stmt != null) {
				try {
					stmt.execute("UNLOCK TABLES");
					System.out.println("Table unlocked.");
				} catch (SQLException e) {
					e.printStackTrace();
					ret = RET_VALUES.GENERIC_ERROR;
				}
			}
		}
		return ret;	
	}

	public int updateStatusOfSimulationsInFile(String table, int newStatus, String filename) {
		int ret = 1;
		Statement stmt = null;
		Connection conn = mServer.getConnection();
		String lock = "LOCK TABLE " + table + " WRITE";
		try {
			conn.setAutoCommit(false);
			stmt = conn.createStatement();
			stmt.execute(lock);
			System.out.println("Locking now....");

			for(int simulation_number : getSimulationNumbersFromFile(filename)) {
				PreparedStatement preparedStatement = conn.prepareStatement("UPDATE "+table+" SET status=? " + "WHERE simulation_number=?");
				preparedStatement.setInt(1,newStatus); 
				preparedStatement.setInt(2,simulation_number); 
				preparedStatement.execute(); 
				preparedStatement.clearParameters();
			}

			conn.commit();
		} catch (SQLException e) {
			e.printStackTrace();
			ret = RET_VALUES.GENERIC_ERROR;
		} finally {
			if (stmt != null) {
				try {
					stmt.execute("UNLOCK TABLES");
					System.out.println("Table unlocked.");
				} catch (SQLException e) {
					e.printStackTrace();
					ret = RET_VALUES.GENERIC_ERROR;
				}
			}
		}
		return ret;	
	}

	private List<Integer> getSimulationNumbersFromFile(String filename) {
		List<Integer> simulationNumberList = new ArrayList<Integer>();
		String line;
		try {
			FileReader fileReader = new FileReader(filename);
			BufferedReader br = new BufferedReader(fileReader);

			while ((line = br.readLine()) != null) {
				String simulationNumberStr = line.split(",")[0].split("=")[1];
				simulationNumberList.add(Integer.parseInt(simulationNumberStr));
			}
		} catch (IOException e) {
			simulationNumberList = null;
		}
		return simulationNumberList;
	}

	public int addResultsToResultsTable() {
		int ret = 1, simulation_number = -1;
		double duration = -1, initial_sir = -1, final_sir = -1, average_time = -1;
		String line;
		Statement stmt = null;
		Connection conn = mServer.getConnection();
		String lock = "LOCK TABLE results WRITE";
		try {
			conn.setAutoCommit(false);
			stmt = conn.createStatement();
			stmt.execute(lock);
			System.out.println("Locking now....");

			try {
				FileReader fileReader = new FileReader(INFO.RESULTS_FILE);
				BufferedReader br = new BufferedReader(fileReader);
				while ((line = br.readLine()) != null) {
					for(String param :  line.split(",")) {
						String[] pair = param.split("=");
						if("simulation_number".equals(pair[0])) {
							simulation_number = Integer.parseInt(pair[1]);
						} else if("duration".equals(pair[0])) {
							duration = Double.parseDouble(pair[1]);
						} else if("initial_sir".equals(pair[0])) {
							initial_sir = Double.parseDouble(pair[1]);
						} else if("sir_final".equals(pair[0])) {
							final_sir = Double.parseDouble(pair[1]);
						} else if("average_time".equals(pair[0])) {
							average_time = Double.parseDouble(pair[1]);
						} else {
							System.out.println("Wrong parameter name: "+pair[0]);
							return  RET_VALUES.GENERIC_ERROR;
						}
					}
					if(simulation_number!=-1 && duration!=-1 && initial_sir!=-1 && final_sir!=-1 && average_time!=-1) {
						System.out.println("Calling insertIntoResultsTable(...)");
						insertIntoResultsTable(conn, simulation_number, duration, initial_sir, final_sir, average_time);
					} else {
						System.out.println("Add result: one of the parameters wasn't initialized!");
						return RET_VALUES.GENERIC_ERROR;
					}
				}
			} catch (FileNotFoundException e) {
				System.out.println("File not found: "+INFO.RESULTS_FILE);
				ret = RET_VALUES.FILE_NOT_FOUND_ERROR;
			} catch (NumberFormatException e) {
				System.out.println("Number Format Exception");
				ret = RET_VALUES.GENERIC_ERROR;
			} catch (IOException e) {
				System.out.println("IO Exception");
				ret = RET_VALUES.GENERIC_ERROR;
			}
			conn.commit();
		} catch (SQLException e) {
			e.printStackTrace();
			ret = RET_VALUES.GENERIC_ERROR;
		} finally {
			if (stmt != null) {
				try {
					stmt.execute("UNLOCK TABLES");
					System.out.println("Table unlocked.");
				} catch (SQLException e) {
					e.printStackTrace();
					ret = RET_VALUES.GENERIC_ERROR;
				}
			}
		}
		return ret;
	}
	
	private int insertIntoResultsTable(Connection conn, int simulation_number, double duration, double initial_sir, double final_sir, double average_time) {
		int ret;
		try {
			Statement st = conn.createStatement();
			int count = st.executeUpdate("INSERT INTO results (simulation_number, wav_duration, initial_sir, final_sir, iteration_average_time) VALUES ("+simulation_number+", "+duration+", "+initial_sir+", "+final_sir+", "+average_time+")");
			System.out.println("Insert into results simulation_number="+simulation_number+", duration="+duration+", initial_sir="+initial_sir+", final_sir="+final_sir+", average_time="+average_time);
			System.out.println(count + " rows were inserted");
			ret = RET_VALUES.SUCCESS;
		} catch (SQLException e) {
			System.out.println("SQL code does not execute.");
			System.err.println("Error message: " + e.getMessage());
			System.err.println("Error number: " + e.getErrorCode());
			ret = RET_VALUES.GENERIC_ERROR;
		}
		return ret;
	}
}
