import pygame
from pygame.locals import *
from pygame.color import *

import pymunk as pm
from pymunk import Vec2d

import random, math


X,Y = 0,1
### Physics collision types
COLLTYPE_DEFAULT = 0
COLLTYPE_MOUSE = 1
COLLTYPE_BALL = 2

SIZE = (800, 600)


def flipy(y):
    """Small hack to convert chipmunk physics to pygame coordinates"""
    return -y+600

def get_ball(balls, p):
    for ball in balls:
        r = ball.radius
        v = ball.body.position
        if (p[X] - v[X]) ** 2 + (p[Y] - v[Y]) ** 2 < r ** 2:
            return ball
    return None


def main():
    pygame.init()
    screen = pygame.display.set_mode((800, 600))
    clock = pygame.time.Clock()
    running = True

    ### Physics stuff
    space = pm.Space()
    space.gravity = Vec2d(0.0, -09.8)

    ### Mouse
    mouse_body = pm.Body()
    mouse_shape = pm.Circle(mouse_body, 3, Vec2d(0,0))
    mouse_shape.collision_type = COLLTYPE_MOUSE
    space.add(mouse_shape)
    mouse_constraint = None

    ## Balls
    balls = []
    ball_image = pygame.image.load("ball.gif")
    ball_radius = ball_image.get_size()[0] / 2.0

    box = pm.Body()
    shape = pm.Segment(box, (0, 0), (SIZE[X], 0), 0.0)
    shape.friction = 0.99
    shape.elasticity = 0.95
    space.add(shape)
    shape = pm.Segment(box, (0, SIZE[Y]), (SIZE[X], SIZE[Y]), 0.0)
    shape.friction = 0.99
    shape.elasticity = 0.95
    space.add(shape)
    shape = pm.Segment(box, (0, 0), (0, SIZE[Y]), 0.0)
    shape.friction = 0.99
    shape.elasticity = 0.95
    space.add(shape)
    shape = pm.Segment(box, (SIZE[X], 0), (SIZE[X], SIZE[Y]), 0.0)
    shape.friction = 0.99
    shape.elasticity = 0.95
    space.add(shape)

    for i in xrange(5):
        radius = random.uniform(40, 100)
        body = pm.Body(radius ** 2, radius ** 4 / 2)
        body.position = (random.uniform(radius, SIZE[X] - radius), random.uniform(radius, SIZE[Y] - radius))
        shape = pm.Circle(body, radius, (0,0))
        shape.friction = 0.5
        shape.elasticity = 0.95
        shape.collision_type = COLLTYPE_BALL
        space.add(body, shape)
        balls.append(shape)

    while running:
        for event in pygame.event.get():
            if event.type == QUIT:
                running = False
            elif event.type == KEYDOWN and event.key == K_ESCAPE:
                running = False
            elif event.type == MOUSEBUTTONDOWN and event.button == 1:
                if mouse_constraint == None:
                    p = (event.pos[X], flipy(event.pos[Y]))
                    ball = get_ball(balls, p)
                    if ball is not None:
                        rel = pm.Vec2d(p[X] - ball.body.position[X], p[Y] - ball.body.position[Y])
                        rel.rotate(-ball.body.angle)
                        mouse_constraint = pm.PinJoint(mouse_body, ball.body, (0, 0), rel)
                        space.add(mouse_constraint)
            elif event.type == MOUSEBUTTONUP and event.button == 1:
                if mouse_constraint != None:
                    space.remove(mouse_constraint)
                    mouse_constraint = None

        clock.tick(50)
        dt = clock.get_time()
        pygame.display.set_caption("fps: " + str(clock.get_fps()))

        p = pygame.mouse.get_pos()
        mouse_body.position = (p[X], flipy(p[Y]))

        ### Update physics
        for x in range(dt):
            space.step(.01)

        ### Draw stuff
        screen.fill(THECOLORS["white"])

        for ball in balls:
            r = ball.radius
            v = ball.body.position
            rot = ball.body.angle
            p = int(v.x), int(flipy(v.y))
            new_image = pygame.transform.rotozoom(ball_image, rot * 180 / math.pi, r / ball_radius)
            new_rect = new_image.get_rect()
            new_rect.center = p
            screen.blit(new_image, new_rect)

        ### Flip screen
        pygame.display.flip()

main()
