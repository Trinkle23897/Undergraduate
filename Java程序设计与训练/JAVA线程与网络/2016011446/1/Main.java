import java.util.*;
import java.io.*;
import java.text.*;

class Main {
	public static void main(String[] args) throws InterruptedException {
		SimpleDateFormat ft = new SimpleDateFormat("[yyyy/MM/dd][hh:mm:ss]");
		while (true) {
			System.out.println(ft.format(new Date()));
			Thread.sleep(1000);
		}
	}
}