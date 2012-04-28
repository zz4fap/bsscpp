package com.bsscpp.jdbctests;

import java.sql.*;

public class SqlServer {
	
	private static final String DRIVER = "com.mysql.jdbc.Driver";
	
	protected String URL;
	
	private final String DB_NAME = "noisybss";
	private final String USER_NAME = "zz4fap";
	private final String PASSWD = "casa1234";
	
	public static final String SIMULATIONS_TABLE_NAME = "simulations";
	public static final String RESULTS_TABLE_NAME = "results";
	public static final String TESTS_TABLE_NAME = "tests_table";
	
	private Connection mConnection;
	
	public static enum SERVER {SERVER1, SERVER2};
	
	public static SqlServer getServer(SERVER server) {
		switch(server) {
		case SERVER1:
			return new SqlServer.FreeMySqlServer();
		case SERVER2:
			return new SqlServer.Db4FreeServer();
		default:
			return null;
		}
	}
	
	public void connectToDb() throws SQLException, InstantiationException, IllegalAccessException, ClassNotFoundException {
		Class.forName(DRIVER).newInstance();
		mConnection = DriverManager.getConnection(URL + DB_NAME, USER_NAME, PASSWD);
	}
	
	public void disconnectFromDb() throws SQLException {
		mConnection.close();
	}
	
	public Connection getConnection() {
		return mConnection;
	}
	
	public static class FreeMySqlServer extends SqlServer {
		public FreeMySqlServer() {
			super.URL = "jdbc:mysql://SQL09.FREEMYSQL.NET:3306/";
		}
	}
	
	public static class Db4FreeServer extends SqlServer {
		public Db4FreeServer() {
			super.URL = "jdbc:mysql://db4free.net:3306/";
		}
	}
}
