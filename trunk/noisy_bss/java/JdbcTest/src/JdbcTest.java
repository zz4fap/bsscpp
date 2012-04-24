import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class JdbcTest {
	
	private static final String URL = "jdbc:mysql://SQL09.FREEMYSQL.NET:3306/";
	private static final String DB_NAME = "noisybss";
	private static final String DRIVER = "com.mysql.jdbc.Driver";
	private static final String USER_NAME = "zz4fap";
	private static final String PASSWD = "casa1234";

	private static final String RECORDINGS_PATH = "/Users/zz4fap/Documents/workspace/bsscpp/noisy_bss/code/";
	private static final String RECORDINGS_FILENAME = "recordings_"; // 1 to 10
	private static final String NOISE_FILENAME = "babble.wav";
	private static final String LEARNING_METHOD = "fixedstepsize";
	
	private static final double[] STEPSIZE = {0.002, 0.004, 0.006, 0.008, 0.01, 0.03};
	private static final int[] N = {256, 256, 512, 512};
	private static final int[] L = {64, 128, 128, 256};
	private static final int EPOCHS = 500;

	public static void main(String[] args) {
		Connection conn = null;

		JdbcTest jdbcTest = new JdbcTest();
		
		try {

			Class.forName(DRIVER).newInstance();
			conn = DriverManager.getConnection(URL + DB_NAME, USER_NAME, PASSWD);
			System.out.println("Connected to the database");
			jdbcTest.populateSimulationsTable(conn);
			conn.close();
			System.out.println("Disconnected from database");
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		

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
	
	private ResultSet selectFromTable(String table, String where_clause) {

		ResultSet res = null;
		Connection conn = null;

		try {
			Class.forName(DRIVER).newInstance();
			conn = DriverManager.getConnection(URL + DB_NAME, USER_NAME, PASSWD);
			System.out.println("Connected to the database");

			try {
				Statement st = conn.createStatement();
				if(where_clause!=null) {
					res = st.executeQuery("SELECT * FROM  "+table+" WHERE "+where_clause+"");
				} else {
					res = st.executeQuery("SELECT * FROM  "+table+"");
				}
				
				while (res.next()) {
					System.out.println(res.toString());
				}

			} catch (SQLException e) {
				System.out.println("SQL code does not execute.");
				System.err.println("Error message: " + e.getMessage());
				System.err.println("Error number: " + e.getErrorCode());
			}

			conn.close();
			System.out.println("Disconnected from database");
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return res;
	}
	
	private void updateTable(String table, String set_clause, String where_clause) {
		Connection conn = null;

		try {
			Class.forName(DRIVER).newInstance();
			conn = DriverManager.getConnection(URL + DB_NAME, USER_NAME, PASSWD);
			System.out.println("Connected to the database");

			try {
				Statement st = conn.createStatement();
				
				int count = 0;
				if(where_clause!=null){
					count = st.executeUpdate("UPDATE "+table+" SET "+set_clause+" WHERE "+where_clause+"");
				} else {
					st.executeUpdate("UPDATE "+table+" SET "+set_clause+"");
				}
				System.out.println(count + " rows were inserted");
			} catch (SQLException e) {
				System.out.println("SQL code does not execute.");
				System.err.println("Error message: " + e.getMessage());
				System.err.println("Error number: " + e.getErrorCode());
			}

			conn.close();
			System.out.println("Disconnected from database");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
