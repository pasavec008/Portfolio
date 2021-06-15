import pygame
from pygame import key
from random import *
pygame.init()
size = 20
players = 2
speed = 1

class Node:
    def __init__(self, x, y, c, previous = None, next = None):
        self.x = x
        self.y = y
        self.c = c
        self.next = next
        self.previous = previous

class Food:
    def __init__(self, size):
        self.size = size
        self.x = randrange(size // 2, size * 50 - size // 2) // size * size
        self.y = randrange(size // 2, size * 50 - size // 2) // size * size
    def drawFood(self):
        pygame.draw.rect(screen, ("yellow"), pygame.Rect(self.x, self.y, self.size, self.size))

class Snake:
    def __init__(self, size, player):
        self.player = player
        self.size = size
        self.colors = ("red", "green", "cyan", "white")
        self.score = 0
        self.font = pygame.font.SysFont('Comic Sans MS', size * 3)
        self.score_text = self.font.render(str(self.score), False, (254, 255, 255))
        self.direction = 4
        self.head = Node(size * 50 // 2, (size * 50 // 4 * (self.player * 2 + 1)) // size * size, self.colors[self.player * 2])
        self.head.next = Node(size * 50 // 2, (size * 50 // 4 * (self.player * 2 + 1)) // size * size, self.colors[self.player * 2 + 1], self.head)
        self.tail = self.head.next

    def drawSnake(self):
        act = self.head
        while act != None:
            pygame.draw.rect(screen, (act.c), pygame.Rect(act.x, act.y, self.size, self.size))
            act = act.next
        screen.blit(self.score_text, (self.size // 2, 0 + self.player * self.size * 45))

    def updateSnake(self, keyName, food):
        if keyName == "up" and self.direction != 2:
            self.direction = 1
        elif keyName == "down" and self.direction != 1:
            self.direction = 2
        elif keyName == "left" and self.direction != 4:
            self.direction = 3
        elif keyName == "right" and self.direction != 3:
            self.direction = 4

        self.head = Node(self.head.x, self.head.y, self.colors[self.player * 2], None, self.head)
        self.head.next.c = self.colors[self.player * 2 + 1]
        self.head.next.previous = self.head

        if self.direction == 1:
            self.head.y -= self.size
        elif self.direction == 2:
            self.head.y += self.size
        elif self.direction == 3:
            self.head.x -= self.size
        elif self.direction == 4:
            self.head.x += self.size

        if self.head.x == food.x and self.head.y == food.y:
            food.x = randrange(food.size // 2, food.size * 50 - food.size // 2) // food.size * food.size
            food.y = randrange(food.size // 2, food.size * 50 - food.size // 2) // food.size * food.size
            self.score += 1
            self.score_text = self.font.render(str(self.score), False, (255, 255, 255))
        else:
            self.tail = self.tail.previous
            self.tail.next = None

pygame.display.set_caption("Super hadík")
if players == 2:
    snakes = (Snake(size, 0), Snake(size, 1))
else:
    snakes = (Snake(size, 0),)
food = Food(size)
screen = pygame.display.set_mode((size * 50, size * 50))
clock = pygame.time.Clock()
done = False
keyName = None
tick = 0

while not done:
    tick += speed
    clock.tick_busy_loop(60)
    for event in pygame.event.get():  
        if event.type == pygame.QUIT:  
            done = True

    if(tick > 7):
        if players == 2:
            speed = snakes[0].score * 0.015 + snakes[1].score * 0.015 + 1
        else:
            speed = snakes[0].score * 0.02 + 1
        keyName = ""
        key = pygame.key.get_pressed()
        if key[pygame.K_UP]:
            keyName = "up"
        elif key[pygame.K_DOWN]:
            keyName = "down"
        elif key[pygame.K_LEFT]:
            keyName = "left"
        elif key[pygame.K_RIGHT]:
            keyName = "right"
        snakes[0].updateSnake(keyName, food)

        if players == 2:
            keyName = ""
            if key[pygame.K_w]:
                keyName = "up"
            elif key[pygame.K_s]:
                keyName = "down"
            elif key[pygame.K_a]:
                keyName = "left"
            elif key[pygame.K_d]:
                keyName = "right"
            snakes[1].updateSnake(keyName, food)
        
        for snake in snakes:
            if snake.head.x < 0 :
                snake.head.x += snake.size * 50
            elif snake.head.x >= snake.size * 50 :
                snake.head.x -= snake.size * 50
            elif snake.head.y < 0 :
                snake.head.y += snake.size * 50
            elif snake.head.y >= snake.size * 50:
                snake.head.y -= snake.size * 50
            elif screen.get_at((snake.head.x, snake.head.y))[:3] in ((255, 255, 255), (255, 0, 0), (0, 255, 0), (0, 255, 255)):
                pygame.time.wait(1500)
                done = True
        
        screen.fill('black')

        for snake in snakes:
            snake.drawSnake()
        food.drawFood()
        pygame.display.flip()
        tick = 0