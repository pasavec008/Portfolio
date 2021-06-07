import pygame
from pygame import key
from random import *
pygame.init()

class Node:
    def __init__(self, x, y, c = 'green', previous = None, next = None):
        self.x = x
        self.y = y
        self.c = c
        self.next = next
        self.previous = previous

class Snake:
    def __init__(self):
        self.score = 0
        self.font = pygame.font.SysFont('Comic Sans MS', 30)
        self.score_text = self.font.render(str(self.score), False, (255, 255, 255))
        self.direction = 4
        self.speed = 1
        self.head = Node(250, 250, 'red')
        self.head.next = Node(250,250, 'green', self.head)
        self.tail = self.head.next
        self.food_x = randrange(5, 495) // 10 * 10
        self.food_y = randrange(5, 495) // 10 * 10

    def drawSnake(self):
        act = self.head
        while act != None:
            pygame.draw.rect(screen, (act.c), pygame.Rect(act.x, act.y, 10, 10))
            act = act.next
        pygame.draw.rect(screen, 'yellow', pygame.Rect(self.food_x, self.food_y, 10, 10))
        screen.blit(self.score_text,(5, 460))

    def updateSnake(self, keyName):
        if keyName == 'up' and self.direction != 2:
            self.direction = 1
        elif keyName == 'down' and self.direction != 1:
            self.direction = 2
        elif keyName == 'left' and self.direction != 4:
            self.direction = 3
        elif keyName == 'right' and self.direction != 3:
            self.direction = 4

        self.head = Node(self.head.x, self.head.y, 'red', None, self.head)
        self.head.next.c = 'green'
        self.head.next.previous = self.head

        if self.direction == 1:
            self.head.y -= 10
        elif self.direction == 2:
            self.head.y += 10
        elif self.direction == 3:
            self.head.x -= 10
        elif self.direction == 4:
            self.head.x += 10

        if self.head.x == self.food_x and self.head.y == self.food_y:
            self.food_x = randrange(5, 495) // 10 * 10
            self.food_y = randrange(5, 495) // 10 * 10
            self.speed += 0.02
            self.score += 1
            self.score_text = self.font.render(str(self.score), False, (255, 255, 255))
        else:
            self.tail = self.tail.previous
            self.tail.next = None

pygame.display.set_caption("Super hadík")
snake = Snake() 
screen = pygame.display.set_mode((500,500))
clock = pygame.time.Clock()
done = False
keyName = None
tick = 0

while not done:
    tick += snake.speed
    clock.tick_busy_loop(60)
    for event in pygame.event.get():  
        if event.type == pygame.QUIT:  
            done = True

    if(tick > 7):
        key = pygame.key.get_pressed()
        if key[pygame.K_UP]:
            keyName = "up"
        elif key[pygame.K_DOWN]:
            keyName = "down"
        elif key[pygame.K_LEFT]:
            keyName = "left"
        elif key[pygame.K_RIGHT]:
            keyName = "right"
        if snake.head.x < 0 or snake.head.x >= 500 or snake.head.y < 0 or snake.head.y >= 500 or screen.get_at((snake.head.x, snake.head.y))[:3] == (0, 255, 0):
            pygame.time.wait(1500)
            done = True
        
        snake.updateSnake(keyName)
        screen.fill('black')
        snake.drawSnake()
        pygame.display.flip()
        tick = 0