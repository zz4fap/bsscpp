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
import javax.naming.directory.InvalidAttributesException;

public class JdbcTest {

	private static final String URL = "jdbc:mysql://SQL09.FREEMYSQL.NET:3306/";
	private static final String DB_NAME = "noisybss";
	private static final String SIMULATIONS_TABLE_NAME = "simulations";
	private static final String DRIVER = "com.mysql.jdbc.Driver";
	private static final String USER_NAME = "zz4fap";
	private static final String PASSWD = "casa1234";

	private static final String RECORDINGS_PATH = "/Users/zz4fap/Documents/workspace/bsscpp/noisy_bss/code/";
	private static final String RECORDINGS_FILENAME = "recordings_"; // 1 to 10
	private static final String NOISE_FILENAME = "babble.wav";
	private static final String LEARNING_METHOD = "fixedstepsize";

	private static final String COMPLETED_SIMULATIONS_PATH = "/Users/zz4fap/Desktop/run_simulations/";
	private static final String SIMULATION_INFO_FILE = "SimulationsInformationForRecordings_";

	private static final double[] STEPSIZE = {0.002, 0.004, 0.006, 0.008, 0.01, 0.03};
	private static final int[] N = {256, 256, 512, 512};
	private static final int[] L = {64, 128, 128, 256};
	private static final int EPOCHS = 500;

	private enum CMD {UNKNOWN, POPULATE, UPDATE, SELECT, RAW, LOCK};

	public static void main(String[] args) {
		Connection conn = null;
		CMD cmd = null;

		JdbcTest jdbcTest = new JdbcTest();

		if((cmd = jdbcTest.handleInputArgs(args))==CMD.UNKNOWN) {
			System.err.println("Wrong command");
			return;
		}
		System.out.println("Command: "+cmd);

		try {
			Class.forName(DRIVER).newInstance();
			conn = DriverManager.getConnection(URL + DB_NAME, USER_NAME, PASSWD);
			System.out.println("Connected to the database");

			switch(cmd) {
			case POPULATE:
				jdbcTest.populateSimulationsTable(conn);
				break;
			case UPDATE:
				int numberOfUpdatedRows = jdbcTest.updateSimulationsTable(conn);
				System.out.println("Number of updated rows: "+numberOfUpdatedRows);
				break;
			case SELECT:
				jdbcTest.selectAndShowFromSimulationsTable(conn, "*", null);
				break;
			case LOCK:
				jdbcTest.lockTable(conn, "tests_table");
				break;
			case RAW:
			case UNKNOWN:
			default:
				System.out.println("Unknown command");
				return;
			}

			conn.close();
			System.out.println("Disconnected from database");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private CMD handleInputArgs(String[] args) {
		CMD cmd = CMD.UNKNOWN;
		if(args.length > 0) {
			String str = args[0];
			str.toLowerCase();
			if("populate".equals(str)) {
				cmd = CMD.POPULATE;
			} else if("update".equals(str)) {
				cmd = CMD.UPDATE;
			} else if("select".equals(str)) {
				cmd = CMD.SELECT;
			} else if("lock".equals(str)) {
				cmd = CMD.LOCK;
			}
		}
		return cmd;
	}

	private int updateSimulationsTable(Connection conn) {
		int[] info = null;
		int numberOfUpdatedRows = 0;
		try {
			for(int i=1; i<=10; i++) {
				info = getLasRunSimulation(COMPLETED_SIMULATIONS_PATH+SIMULATION_INFO_FILE+i+".txt");

				if(info!=null){
					for(int j=1; j<=info[0]; j++) { //iterates over the set of wave files.
						for(int k=1; k<=24; k++) {

							int simulation_number = ((i-1)*2880 + ((j-1)*24 + k));

							numberOfUpdatedRows++;

							updateTable(conn, SIMULATIONS_TABLE_NAME, "status=2", "simulation_number="+simulation_number);

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

	private void populateSimulationsTable(Connection conn) {
		int counter = 1;
		List<String> recordings;

		for(int i=1; i<=10; i++) {
			recordings = openRecordingsFile(RECORDINGS_FILENAME + i);
			for(String recording : recordings) {

				for(double stepsize : STEPSIZE) {

					for(int n=0; n<N.length; n++) {

						insertIntoSimulationsTable(conn, recording, NOISE_FILENAME, stepsize, N[n], L[n], LEARNING_METHOD, EPOCHS);

						System.out.println("counter: " + counter
								+ " wav_filename: " + recording
								+ " noise_filename: " + NOISE_FILENAME
								+ " stepsize: " + stepsize
								+ " N: " + N[n]
										+ " L: " + L[n]
												+ " learning_method: " + LEARNING_METHOD
												+ " epochs: " + EPOCHS);

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
			FileInputStream fstream = new FileInputStream(RECORDINGS_PATH + filename);
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

	private void selectAndShowFromSimulationsTable(Connection conn, String what, String where_clause) {
		ResultSet res = selectFromTable(conn, SIMULATIONS_TABLE_NAME, what, where_clause);
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

	private void lockTable(Connection conn, String table) {
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
