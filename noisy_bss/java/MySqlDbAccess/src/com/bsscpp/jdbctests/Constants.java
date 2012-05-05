package com.bsscpp.jdbctests;

public class Constants {
	
	public static class RET_VALUES {
		public static final int SUCCESS = 0;
		public static final int ERROR = -1;
	}
	
	public static class INFO {
		public static final String RECORDINGS_PATH = "/Users/zz4fap/Documents/workspace/bsscpp/noisy_bss/code/";
		public static final String RECORDINGS_FILENAME = "recordings_"; // 1 to 10
		public static final String NOISE_FILENAME = "babble.wav";
		public static final String LEARNING_METHOD = "fixedstepsize";

		public static final String COMPLETED_SIMULATIONS_PATH = "/Users/zz4fap/Desktop/run_simulations/";
		public static final String SIMULATION_INFO_FILE = "SimulationsInformationForRecordings_";
		public static final String UPDATE_SIMULATION_INFO_FILE = "update_simulation_info.txt";
		public static final String AVAILABLE_SIMULATIONS_FILE = "available_simulations.txt";
		public static final String RETRIEVED_SIMULATIONS_FILE = "retrieved_simulations.txt";

		public static final double[] STEPSIZE = {0.002, 0.004, 0.006, 0.008, 0.01, 0.03};
		public static final int[] N = {256, 256, 512, 512};
		public static final int[] L = {64, 128, 128, 256};
		public static final int EPOCHS = 500;
	}
	
	public static enum CMD {
		UNKNOWN, POPULATE, UPDATE, SELECT, RAW, LOCK, TEST_CONN, RETRIEVE, UPDATE_STATUS, UPDATE_STATUS_OF_FILE;
		
		public static String getCmds() {

			String values = "";
			for (CMD cmd : CMD.values()) {
				values += cmd.toString()+"\n";
			}
			return values;
		};}
	
	public static String HELP_MSG = "Usage:\njava JdbcTest [server] [command]";
}
