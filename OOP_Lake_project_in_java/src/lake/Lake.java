package lake;
import lake.animals.*;
import lake.plants.*;
import java.util.ArrayList;

final public class Lake {
	private static Lake instance;
	private int temperature = 20;
	private int nitrogenLevel = 0;
	private int numberOfAnimals = 0;
	private int numberOfPlants = 0;
	private int lifeCycle = 0;
	private ArrayList<Animal> animals = new ArrayList<Animal>();
	private ArrayList<Plant> plants = new ArrayList<Plant>();
	
	
	public static Lake getInstance() {
		if(instance == null)
			return (instance = (new Lake()));
		return instance;
	}
	
	private Lake() {
	}
	
	public void addAnimal(Animal baby) {
		this.animals.add(baby);
		this.numberOfAnimals++;
	}
	
	public void addPlant(Plant baby) {
		this.plants.add(baby);
		this.numberOfPlants++;
	}

	public void startingOrganisms() {
		for(int i = 0; i < 2; i++) {
			addAnimal(new Turtle());
		}
		for(int i = 0; i < (int)(Math.random() * 5) + 3; i++) {
			addAnimal(new Frog());
			addPlant(new Reed());
		}
		for(int i = 0; i < (int)(Math.random() * 6) + 4; i++) {
			addAnimal(new Snail());
		}
		for(int i = 0; i < (int)(Math.random() * 4) + 5; i++) {
			addAnimal(new Fish());
		}
		for(int i = 0; i < (int)(Math.random() * 7) + 3; i++) {
			addAnimal(new Shrimp());
			addPlant(new Algae());
		}
	}
	
	public void lifeCycle() {
		this.lifeCycle++;
		for (int i = 0; i < plants.size(); i++) {
			nitrogenLevel += plants.get(i).lifeCycle(nitrogenLevel, temperature);
			if(plants.get(i).getHealth() <= 0) {
				System.out.println("Zomrel/a " + plants.get(i).getName());
				plants.remove(i--);
				numberOfPlants--;
				nitrogenLevel+=1;
			}
			else if(plants.get(i).getReproductionCooldown() == 0) {
				this.plants.add(plants.get(i).reproduce());
				this.numberOfPlants++;
				nitrogenLevel-=1;
			}
				
	    }
		for (int i = 0; i < animals.size(); i++) {
			nitrogenLevel += animals.get(i).lifeCycle(temperature, nitrogenLevel);
			if(animals.get(i).getHealth() <= 0) {
				System.out.println("Zomrel/a " + animals.get(i).getName());
				animals.remove(i--);
				numberOfAnimals--;
				nitrogenLevel+=3;
			}
			else{
				if(animals.get(i).getHealth() == 100 && animals.get(i).getReproductionCooldown() == 0) {
					this.animals.add(animals.get(i).mate());
					this.numberOfAnimals++;
				}
			}
			
	    }
		this.temperature += (this.numberOfAnimals + this.numberOfPlants) / 35;
		if(this.temperature > 20)
			this.temperature -= Math.random()*3;
		System.out.println("\nVek jazera: " + this.lifeCycle);
		System.out.println("Pocet organizmov: " + (this.numberOfAnimals + this.numberOfPlants));
		System.out.println("Pocet zivocichov: " + this.numberOfAnimals);
		System.out.println("Pocet rastlin: " + this.numberOfPlants);
		System.out.println("Teplota jazera: " + this.temperature);
		System.out.println("Znecistenie jazera: " + this.nitrogenLevel + "\n");
	}


}
