import pathlib
import time
import os
import sys
import random
from tkinter import *

##################################################
# Initialize C++ bindings
##################################################

import py_pbd_simulation as pbd
# this is all what is needed to have access to exposed c++ 

##################################################
# Class managing the UI
##################################################

couleurs = ["red", "green", "blue", "yellow", "orange", "purple", "pink", "brown", "gray", "black", "white", "cyan", "magenta", "lightblue", "olive"]

#variables globales, initialisées comme-ci mais modifiables

masse = 15.0 #kg 
rayon = 0.5 #mètres
vx = 0.0
vy = 0.0



class ParticleUI :
    def __init__(self) :
        # Création du contexte
        self.context = pbd.Context(200)
        self.width = 1000
        self.height = 1000

        # Simulation physique entre :  [-world_x_max,world_x_max] / [-world_y_max,world_y_max]
        self.world_x_max = 10
        self.world_y_max = 10

        self.window = Tk()

        # Création de la simulation
        self.canvas = Canvas(self.window,width=self.width,height=self.height)
        self.canvas.pack()

        # Boutons pour augmenter / diminuer le rayon
        button = Button(self.window,width = 20,height = 2, text="Augmenter Rayon de 10cm", command=self.uprayon)
        button.place(x=0, y=0)
        button = Button(self.window,width = 20,height = 2, text="Diminuer Rayon de 10cm", command=self.downrayon)
        button.place(x=0, y=40)

        # Boutons pour augmenter / diminuer la masse
        button = Button(self.window,width = 20,height = 2, text="Augmenter Masse de 2 kg", command=self.upmasse)
        button.place(x=250, y=0)
        button = Button(self.window,width = 20,height = 2, text="Diminuer Masse de 2 kg", command=self.downmasse)
        button.place(x=250, y=40)

        # Boutons pour augmenter / diminuer la vitesse x
        button = Button(self.window,width = 20,height = 2, text="Augmenter vitesse x de 0.5", command=self.upvitessex)
        button.place(x=500, y=0)
        button = Button(self.window,width = 20,height = 2, text="Diminuer vitesse x de 0.5", command=self.downvitessex)
        button.place(x=500, y=40)

        # Boutons pour augmenter / diminuer la vitesse y
        button = Button(self.window,width = 20,height = 2, text="Augmenter vitesse y de 0.5", command=self.upvitessey)
        button.place(x=750, y=0)
        button = Button(self.window,width = 20,height = 2, text="Diminuer vitesse y de 0.5", command=self.downvitessey)
        button.place(x=750, y=40)


        

        # Ajout d'une première particule en mouvement
        self.addParticle((0,0), 2.0, 100.0, (0.0, 0.0), "orange")

        #Création de la boîte fermée 
        self.addPlan((-6,-8),(0,1)) #Sol
        self.addPlan((-9.5,0),(1,0)) #Mur Gauche
        self.addPlan((10,0),(-1,0)) #Mur droite
        self.addPlan((-5,8),(0,-1)) #Toit

        #Création d'une sphère fixe pour vérifier nos intéractions particule / Sphère
        self.addSphere((4,4),2)

        
        # Initialize Mouse and Key events
        self.canvas.bind("<Button-1>", lambda event: self.mouseCallback(event))
        self.window.bind("<Key>", lambda event: self.keyCallback(event)) # bind all key
        self.window.bind("<Escape>", lambda event: self.enterCallback(event)) 
        # bind specific key overide default binding

        
        

    def launchSimulation(self) :
        # lancement de l'animation
        self.animate()
        # launch UI event loop
        self.window.mainloop()

    def animate(self) :
        """ animation loop """
        # Dans ce code, on va appliquer toutes les modifications physiques, de position et de vitesse
        # Cela permettra une bonne simulation 

        
        for i in range(6) :
            self.context.updatePhysicalSystem(0.016/6.0) #Update de l'animation physique à chaque pas dt

        for i in range(self.context.num_particles()):
            # Transormation des coordonnées dans la bonne forme, à l'aide de nos fonctions définies ci-dessous
            particle = self.context.particle(i)
            pmin = (particle.position.x - particle.radius, 
                    particle.position.y - particle.radius)
            pmax = (particle.position.x + particle.radius, 
                    particle.position.y + particle.radius)
            self.canvas.coords(particle.draw_id, 
                               *self.worldToView(pmin), 
                               *self.worldToView(pmax) )
            # END TODO
        self.window.update()
        self.window.after(16, self.animate)

    # Conversion depuis la simulation vers le dessin correspondant, et vis-versa
    def worldToView(self, world_pos) :
        return ( self.width *(0.5 + (world_pos[0]/self.world_x_max) * 0.5),
                 self.height *(0.5 - (world_pos[1]/self.world_y_max) * 0.5)) 
    def viewToWorld(self, view_pos) :
        return ( self.world_x_max * 2.0 * (view_pos[0]/self.width - 0.5) ,
                 self.world_y_max * 2.0 * (0.5-view_pos[1]/self.height))  


    def addParticle(self, world_pos, radius, mass, velocity, color) :
        # Fonction d'ajout d'une particule dans le canva, et dans le contexte, en transformant nos coordonnées
        draw_id = self.canvas.create_oval(0,0,0,0,fill=color)
        self.context.addParticle(pbd.Vec2(*world_pos), radius, mass, pbd.Vec2(*velocity), draw_id)

    #Fonctions utilisées dans les boutons

    #Fonctions rayon
    def uprayon(self):
        global rayon
        rayon = rayon + 0.1
    def downrayon(self):
        global rayon
        if rayon>=0.1:
            rayon = rayon - 0.1

    #Fonctions masse
    def upmasse(self):
        global masse
        masse = masse + 2
    def downmasse(self):
        global masse
        if masse>2:
            masse = masse - 2

    #Fonctions vitesse
    def upvitessex(self):
        global vx
        vx = vx + 0.5
    def downvitessex(self):
        global vx
        vx = vx - 0.5
    def upvitessey(self):
        global vy
        vy = vy + 0.5
    def downvitessey(self):
        global vy
        vy = vy - 0.5


    #Ajout d'un plan au context et au dessin à partir d'un de ses points et d'un vecteur normal
    def addPlan(self, point, normale) : 

        self.context.addPlan(pbd.Vec2(*point),pbd.Vec2(*normale)) #Ajout dans le context


        x1 = point[0]
        y1 = point[1]
        
        x2 = x1 - normale[1]
        y2 = y1 +  normale[0]
        point_2 = (x2,y2) # Point 2 sur la droite, permettant de la tracer

        #Transformation dans la bonne forme
        a = self.worldToView(point)
        b = self.worldToView(point_2)

        direction_vector = (b[0] - a[0], b[1] - a[1])
        #Tracer en négatif et en positif, pour ne pas voir les bords graphiquement
        end_point1 = (a[0] - direction_vector[0] * 1000, a[1] - direction_vector[1] * 1000)
        end_point2 = (a[0] + direction_vector[0] * 1000, a[1] + direction_vector[1] * 1000)

        # Dessin de la ligne vue comme infinie graphiquement
        self.canvas.create_line(end_point1[0], end_point1[1], end_point2[0], end_point2[1], fill="brown", width=2)

        
    #Ajout d'une Sphère fixe sur le Canvas et dans le context
    def addSphere(self,centre,rayon) : 

        #Tracer dans le canvas
        point1 = self.worldToView((centre[0]-rayon,centre[1]-rayon))
        point2 = self.worldToView((centre[0]+rayon,centre[1]+rayon))
        draw_id = self.canvas.create_oval(point1,point2,fill="red")

        #Ajout au context
        self.context.addSphere(pbd.Vec2(*centre), rayon)


    # Fonctions permettant l'ajout au clic de particules
    def mouseCallback(self, event): #Ajout une particule à partir de ses caractéristiques et d'une couleur aléatoire
        global rayon, masse, vx, vy
        couleur = random.choice(couleurs)
        self.addParticle(self.viewToWorld((event.x,event.y)), rayon, masse, (vx, vy), couleur)
        
    
    def keyCallback(self, event):
        print(repr(event.char))
    def enterCallback(self, event):
        self.window.destroy()


#Lancement de la Simulation
gui = ParticleUI()
gui.launchSimulation()