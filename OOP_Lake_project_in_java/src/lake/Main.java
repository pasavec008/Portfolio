package lake;

import java.util.Scanner;
import java.util.Timer;
import java.util.TimerTask;

public class Main {

	public static void main(String[] args) {
		Scanner scanner = new Scanner(System.in);
		Lake lakeInstance = Lake.getInstance();
		lakeInstance.startingOrganisms();
		System.out.println("Zaciatok simulacie.\nPre zastavenie simulacie napiste stop.");
		Timer timer = new Timer();
		TimerTask cycle = new TimerTask() {
		    public void run() {
		    	lakeInstance.lifeCycle();
		    }
		};
		timer.schedule(cycle, 0, 5000);
		String control = scanner.nextLine();
		while(control.toUpperCase().contentEquals("STOP") != true)
			control = scanner.nextLine();
		timer.cancel();
		System.out.println("Simulacia ukoncena.");
		scanner.close();
	}
}
