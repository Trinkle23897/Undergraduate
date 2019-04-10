import java.util.*;
import java.io.*;

class Singleton {
	private static Singleton instance = new Singleton();
	private Singleton() {}
	public static Singleton getInstance() {
		return instance;
	}
}

class Main {
	public static void main(String[] args) {
		Singleton single = Singleton.getInstance();
		Singleton single2 = Singleton.getInstance();
		System.out.println(single.equals(single2));
		// Singleton single3 = new Singleton();
	}
}