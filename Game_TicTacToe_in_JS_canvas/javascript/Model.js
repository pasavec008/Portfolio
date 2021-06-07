class Model{
    platno;
    context;
    view;
    screen;
    keys;
    area;
    mode = 0;
    music = -1;
    meadow = new Audio("../music/theMeadow.mp3");;
    forest = new Audio("../music/natureSoundsForStressRelief.mp3");;

    constructor(platno, context, screen){
        this.platno = platno;
        this.context = context;
        this.screen = screen;
        this.view = new View(platno, context, screen);
    }

    sizeChoice(x, y){
        if(x > this.screen.width / 2 - 400 && x < this.screen.width / 2 - 400 + 250 && y > this.screen.height / 2 - 125 && y < this.screen.height / 2 - 125 + 250){
            this.setupGame(9);
            return 1;
        }
        if(x > this.screen.width / 2 - 125 && x < this.screen.width / 2 - 125 + 250 && y > this.screen.height / 2 - 125 && y < this.screen.height / 2 - 125 + 250){
            this.setupGame(15);
            return 1;
        }
        if(x > this.screen.width / 2 + 150 && x < this.screen.width / 2 + 150 + 250 && y > this.screen.height / 2 - 125 && y < this.screen.height / 2 - 125 + 250){
            this.setupGame(23);
            return 1;
        }
        return 0;
    }

    bushChoice(x){
        if(x < this.screen.width / 2)
            this.area.texture0.src = "../textures/bush2.png";
        else
            this.area.texture0.src = "../textures/grass2.png";
    }

    startMusic(){
        this.meadow.play();
        this.meadow.loop = true;
        this.meadow.volume = 0.7;

        this.forest.play();
        this.forest.loop = true;
        this.forest.volume = 0.2;
    }

    getFromMouseEvent(event){
        if(this.mode == 0){
            if(this.music == -1){
                this.startMusic();
                this.music = 1;
            }
            if(this.sizeChoice(event.offsetX, event.offsetY))
                this.mode = 2;
        }
        else if(this.mode == 2){
            this.bushChoice(event.offsetX)
            this.mode = 1;
        }
        else if(this.mode == 3){
            this.mode = 0;
        }
        else{
            this.area.click(event.offsetX, event.offsetY, this.mode, this.meadow, this.forest, this.screen, this);
            this.mode = this.area.mode;
        }  
    }

    setupGame(size){
        this.area = new Area(size, this.music);
        this.view.setSize(this.area, size);
    }

    modelLoop(){
        this.view.viewLoop(this.area, this.mode);
    }
}