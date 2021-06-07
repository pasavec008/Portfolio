import matplotlib.pyplot as plt
import random
import numpy
import math

def calculate_distance(x1, y1, x2, y2):
     distance = math.sqrt((x2 - x1)**2 + (y2 - y1)**2)
     return distance

def classify(k, x_new, y_new, color_new, x_dataset, y_dataset, color_dataset, mode, dataset_size):
    if mode:
        distances = []
        neighbours = [0, 0, 0, 0]
        for i in range(dataset_size):
            distances.append([calculate_distance(x_dataset[i], y_dataset[i], x_new, y_new), color_dataset[i]])
        distances = sorted(distances)

        for i in range(k):
            neighbours[distances[i][1] - 1] += 1
        
        max_neighbour_value = max(neighbours)
        max_neighbour_index = numpy.argmax(neighbours)

        duplicate = 1

        for i in range(len(neighbours)):
            if neighbours[i] == max_neighbour_value and i != max_neighbour_index:       #ak najdem, ze je rovnaky pocet susedov pri dvoch farbach
                for i in range(k):                                                      #tak preloopujem cez susedov a priradi sa farba najblizsieho, co ma tiez max
                    if neighbours[distances[i][1] - 1] == max_neighbour_value:
                        clasified_color = distances[i][1]
                        duplicate = 0
                        break
                break

        if duplicate:
            clasified_color = max_neighbour_index + 1

        if clasified_color == color_new:
            success = 1
        else:
            success = 0
        
        return success, clasified_color

    return 1, color_new

def generate_point(point_type, generated):
    if point_type == 1: #red
        color = 1
        if random.random() < 0.99:
            x = random.randrange(-5000, 500)
            y = random.randrange(-5000, 500)
        else:
            x = random.randrange(-5000, 5001)
            y = random.randrange(-5000, 5001)    
    elif point_type == 2:   #green
        color = 2
        if random.random() < 0.99:
            x = random.randrange(-501, 5001)
            y = random.randrange(-5000, 500)
        else:
            x = random.randrange(-5000, 5001)
            y = random.randrange(-5000, 5001) 
    elif point_type == 3:   #blue
        color = 3
        if random.random() < 0.99:
            x = random.randrange(-5000, 500)
            y = random.randrange(-501, 5001)
        else:
            x = random.randrange(-5000, 5001)
            y = random.randrange(-5000, 5001) 
    elif point_type == 4:   #purple
        color = 4
        if random.random() < 0.99:
            x = random.randrange(-501, 5001)
            y = random.randrange(-501, 5001)
        else:
            x = random.randrange(-5000, 5001)
            y = random.randrange(-5000, 5001) 
    else:
        print("Error 1")

    if generated[y][x] == 1:
        #print("Duplikat", x, y)
        return generate_point(point_type, generated)

    return x, y, color
        
def init_dataset():
    x = [-4500, -4100, -1800, -2500, -2000, 4500, 4100, 1800, 2500, 2000, -4500, -4100, -1800, -2500, -2000, 4500, 4100, 1800, 2500, 2000]
    y = [-4400, -3000, -2400, -3400, -1400, -4400, -3000, -2400, -3400, -1400, 4400, 3000, 2400, 3400, 1400, 4400, 3000, 2400, 3400, 1400]
    color = [1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4]

    return x, y, color

def main():
    #random.seed(27)
    k = 3
    total_success = 0
    pocet_bodov = 2000
    x_dataset, y_dataset, color_dataset = init_dataset()
    generated = numpy.zeros( (10000, 10000) )
    dataset_size = 20

    for i in range(pocet_bodov):
        print(i)
        point_type = i % 4 + 1
        x_new, y_new, color_new = generate_point(point_type, generated)
        generated[x_new][y_new] = 1
        success, color_new = classify(k, x_new, y_new, color_new, x_dataset, y_dataset, color_dataset, 1, dataset_size)
        x_dataset.append(x_new)
        y_dataset.append(y_new)
        dataset_size += 1
        color_dataset.append(color_new)
        total_success += success

    print("% uspesnost pokusu s k =", k, " --> ", total_success / pocet_bodov * 100, "%")

    for i in range(dataset_size):
        if color_dataset[i] == 1:
            color_dataset[i] = "red"
        elif color_dataset[i] == 2:
            color_dataset[i] = "green"
        elif color_dataset[i] == 3:
            color_dataset[i] = "blue"
        elif color_dataset[i] == 4:
            color_dataset[i] = "purple"
    plt.scatter(x_dataset, y_dataset, 3, color_dataset)
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.show()

main()