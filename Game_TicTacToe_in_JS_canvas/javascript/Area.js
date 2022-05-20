class Area{
    size;
    tiles;
    playerNumber = 1;
    startX;
    startY;
    tileSize;
    frameCounter = 0;
    frameChoice = 0;
    mode = 1;
    music;
    bush = new Image();
    grass = new Image();
    texture0 = new Image();
    texture1 = new Image();
    texture2 = new Image();
    winFrame = new Image();

    musicON = new Image();
    musicOFF = new Image();

    constructor(size, music){
        this.music = music;
        this.size = size;
        this.tiles = [];
        for(var i = 0; i < size; i++){
            this.tiles[i] = [];
        }
        for(var i = 0; i < size; i++){
            for(var y = 0; y < size; y++){
                this.tiles[i][y] = 0;
            }
        }

        this.bush.src = "textures/bush2.png";
        this.grass.src = "textures/grass2.png";
        this.texture0.src = "textures/bush2.png";
        this.texture1.src = "textures/bee2.png";
        this.texture2.src = "textures/daisy2.png";
        this.winFrame.src = "textures/frame.png";

        this.musicON.src = "textures/soundON.png";
        this.musicOFF.src = "textures/soundOFF.png";
    }

    setStart(x, y, tileSize){
        this.startX = x;
        this.startY = y;
        this.tileSize = tileSize;
    }

    winControl(tileX, tileY){
        //4 kontroly, 2x diagonala, 1x hore/dole, 1x vpravo/vlavo
        var counter = 1;
        var i = 1;


        //diagonala zacinajuca vlavo hore
        while(tileY + i < this.size && tileX + i < this.size && this.tiles[tileY + i][tileX + i] == this.playerNumber){
            counter++;
            i++;
        }
        i = 1;
        while(tileY - i >= 0 && tileX - i >= 0 && this.tiles[tileY - i][tileX - i] == this.playerNumber){
            counter++;
            i++;
        }
        if(counter >= 5){
            this.mode = 3;
            return 1;
        }
        counter = 1;
        i = 1;
            

        
        //opacna diagonala
        console.log(i);
        while(tileY - i >= 0 && tileX + i < this.size && this.tiles[tileY - i][tileX + i] == this.playerNumber){
            counter++;
            i++;
        }
        i = 1;
        while(tileY + i < this.size && tileX - i >= 0 && this.tiles[tileY + i][tileX - i] == this.playerNumber){
            counter++;
            i++;
        }
        if(counter >= 5){
            this.mode = 3;
            return 1;
        }
        counter = 1;
        i = 1;



        //vpravo/vlavo
        while(tileX + i < this.size && this.tiles[tileY][tileX + i] == this.playerNumber){
            counter++;
            i++;
        }
        i = 1;
        while(tileX - i >= 0 && this.tiles[tileY][tileX - i] == this.playerNumber){
            counter++;
            i++;
        }
        if(counter >= 5){
            this.mode = 3;
            return 1;
        }
        counter = 1;
        i = 1;



        //hore/dole
        while(tileY + i < this.size && this.tiles[tileY + i][tileX] == this.playerNumber){
            counter++;
            i++;
        }
        i = 1;
        while(tileY - i >= 0 && this.tiles[tileY - i][tileX] == this.playerNumber){
            counter++;
            i++;
        }
        if(counter >= 5){
            this.mode = 3;
            return 1;
        }
    }

    click(x, y, mode, meadow, forest, screen, model){
        if(x > screen.width - 100 && x < screen.width - 30 && y > 30 && y < 230){
            if(this.music == 1){
                this.music = 0;
                model.music = 0;
                meadow.pause();
                forest.pause();
            }
            else{
                this.music = 1;
                model.music = 1;
                meadow.play();
                forest.play();
            }
        }

        if(mode != 3){
            var tileX = Math.floor((x - this.startX) / this.tileSize);
            var tileY = Math.floor((y - this.startY) / this.tileSize);
            if(this.tiles[tileY][tileX] == 0){
                this.tiles[tileY][tileX] = this.playerNumber;
                this.winControl(tileX, tileY);
                if(this.playerNumber == 1)
                    this.playerNumber = 2;
                else
                    this.playerNumber = 1;
            }
        }
    }

    countAnimationFrame(){
        this.frameCounter += 0.25;
        this.frameChoice = Math.floor(this.frameCounter);
    }

    draw(context, screen){
        //music on/off
        if(this.music == 1)
            context.drawImage(this.musicON, 0 + this.frameChoice % 3 * 70, 0, 70, 200, screen.width - 100, 30, 70, 200);
        else
            context.drawImage(this.musicOFF, screen.width - 100, 30);

        if(this.playerNumber == 1)
            context.drawImage(this.texture1, 0 + this.frameChoice % 2 * 300, 0, 300, 300, 10, 10, 150, 150);
        else
            context.drawImage(this.texture2, 0 + this.frameChoice % 2 * 300, 0, 300, 300, 10, 10, 150, 150);

        for(var i = 0; i < this.size; i++){
            for(var y = 0; y < this.size; y++){
                if(this.tiles[i][y] == 0)
                    context.drawImage(this.texture0, 0 + this.frameChoice % 2 * 300, 0, 300, 300, this.startX + y * this.tileSize, this.startY + i * this.tileSize, this.tileSize, this.tileSize);
                if(this.tiles[i][y] == 1)
                    context.drawImage(this.texture1, 0 + this.frameChoice % 2 * 300, 0, 300, 300, this.startX + y * this.tileSize, this.startY + i * this.tileSize, this.tileSize, this.tileSize);
                if(this.tiles[i][y] == 2)
                    context.drawImage(this.texture2, 0 + this.frameChoice % 2 * 300, 0, 300, 300, this.startX + y * this.tileSize, this.startY + i * this.tileSize, this.tileSize, this.tileSize);
            }
        }
    }

    drawBushChoice(context, screen){
        context.drawImage(this.bush, 0 + this.frameChoice % 2 * 300, 0, 300, 300, screen.width / 3 - this.bush.width / 4, screen.height / 2 - this.bush.height / 4, 300, 300);
        context.drawImage(this.grass, 0 + this.frameChoice % 2 * 300, 0, 300, 300, screen.width / 3 * 2 - this.grass.width / 4, screen.height / 2 - this.grass.height / 4, 300, 300);
    }

    drawWin(context, screen){
        if(this.playerNumber == 1){
            context.drawImage(this.winFrame, screen.width / 2 - this.winFrame.width / 2, screen.height / 2 - this.winFrame.height / 2);
            context.drawImage(this.texture2, 0 + this.frameChoice % 2 * 300, 0, 300, 300, screen.width / 2 - 400, screen.height / 2 - this.texture2.height / 2 + 30, 220, 220);
        }
        else{
            context.drawImage(this.winFrame, screen.width / 2 - this.winFrame.width / 2, screen.height / 2 - this.winFrame.height / 2);
            context.drawImage(this.texture1, 0 + this.frameChoice % 2 * 300, 0, 300, 300, screen.width / 2 - 400, screen.height / 2 - this.texture1.height / 2 + 30, 220, 220);
        }
    }
}