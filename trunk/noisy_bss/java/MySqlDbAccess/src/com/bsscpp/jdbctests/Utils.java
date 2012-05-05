package com.bsscpp.jdbctests;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

public class Utils {
	
	public static boolean writeToFile(String filename, String text, boolean append) {
		boolean ret = true;
		try {
			BufferedWriter out = new BufferedWriter(new FileWriter(filename, append));
			out.write(text);
			out.close();
		} catch (IOException e) {
			e.printStackTrace();
			ret = false;
		}
		return ret;
	}
}
