import java.util.*;
import java.io.*;

// 2.1
class Man implements Person {
	private String name;
	private String description;
	private int count;
	Man() { name = ""; description = ""; count = 0; }
	Man(String myname, String mydesc) {
		name = myname;
		description = mydesc;
		count = 0;
	}
	public String getName() { return name; }
	public String getDescription() { return description; }
	public int changeSomething() { return --count; }
	int changeSomethingPlus() { return ++count; }
	void move() { System.out.println("I am moving..."); }
}
// 2.2
class SuperMan extends Man {
	SuperMan() { super("superMan", "I can fly"); }
	SuperMan(String name, String desc) { super(name, desc); }
	void move() { System.out.println("I am flying..."); }
	void fly() { System.out.println("fly, I am a SuperMan"); }
	public int changeSomething() { return super.changeSomethingPlus(); }
}

class Main {
	static void test23() {
		Man man=new Man("man","nothing");
		System.out.println(man.getName());
		System.out.println(man.getDescription());
		System.out.println(man.changeSomething());
		man.move();
		SuperMan superman=new SuperMan("superman","nothing");
		System.out.println(superman.getName());
		System.out.println(superman.getDescription());
		System.out.println(superman.changeSomething());
		superman.move();
		superman.fly();
		Person pman=new Man("pman","nothing");
		System.out.println(pman.getName());
		System.out.println(pman.getDescription());
		System.out.println(pman.changeSomething());
		// pman.move();
		Person psman=new SuperMan("psman","nothing");
		System.out.println(psman.getName());
		System.out.println(psman.getDescription());
		System.out.println(psman.changeSomething());
		// psman.move();
		// psman.fly();
		Man msMan=new SuperMan("msMan","nothing");
		System.out.println(msMan.getName());
		System.out.println(msMan.getDescription());
		System.out.println(msMan.changeSomething());
		msMan.move();
		// msMan.fly();
	}
	static void test241() {
		Man man=new Man("man","nothing");
		SuperMan sman=(SuperMan)man;
	}
	static void test242() {
		Man man=new SuperMan("superman","nothing");
		SuperMan sman=(SuperMan)man;
		Man man2=(Man)sman;
	}
	static void test243() {
		Person man=new Man("man","nothing");
		SuperMan sman=(SuperMan)man;
	}
	public static void main(String[] args) {
		test23();
		test241();
		test242();
		test243();
	}
}