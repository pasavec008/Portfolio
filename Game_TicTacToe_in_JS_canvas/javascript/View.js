class View{
    platno;
    context;
    size;
    screen;
    tileSize;
    background = new Image();
    choiceSize = new Image();
    choiceBush = new Image();

    constructor(platno, context, screen){
        this.platno = platno;
        this.context = context;
        this.screen = screen;
        this.background.src = "backgrounds/2.jpg";
        this.choiceSize.src = "textures/choice.png"

    }

    setSize(area, size){
        this.size = size;
        this.tileSize = Math.floor((this.screen.height - 40) / size);
        area.setStart(this.screen.width / 2 - this.tileSize * (size / 2), this.screen.height / 2 - this.tileSize * (size / 2), this.tileSize);
    }

    viewLoop(area, mode){
        this.context.clearRect(0, 0, this.platno.width, this.platno.height);
        this.context.drawImage(this.background, this.background.width / 2 - this.screen.width / 2, this.background.height / 2 - this.screen.height / 2, this.screen.width, this.screen.height, 0, 0, this.screen.width, this.screen.height)
        if(mode == 0){
            this.context.drawImage(this.choiceSize, 0, 0, 250, 250, this.screen.width / 2 - 400, this.screen.height / 2 - 125, 250, 250);
            this.context.drawImage(this.choiceSize, 250, 0, 250, 250, this.screen.width / 2 - 125, this.screen.height / 2 - 125, 250, 250);
            this.context.drawImage(this.choiceSize, 500, 0, 250, 250, this.screen.width / 2 + 150, this.screen.height / 2 - 125, 250, 250);
        }
        if(mode == 2){
            area.countAnimationFrame();
            area.drawBushChoice(this.context, this.screen);
        }
        if(mode == 1 || mode == 3){
            area.countAnimationFrame()
            area.draw(this.context, this.screen);
        }
        if(mode == 3){
            area.countAnimationFrame()
            area.drawWin(this.context, this.screen);
        }
    }
}