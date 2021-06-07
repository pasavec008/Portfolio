class Controller{
    platno;
    context;
    model;

    constructor(){
        
    }

    eventListener(model){
        model.keys = [];
        window.onkeydown = function(event){
            model.keys[event.keyCode] = true;
        }
        window.onkeyup = function(event){
            model.keys[event.keyCode] = false;
        }
        window.onclick = function(event){
            model.getFromMouseEvent(event);
        }
        
    }

    start(platno){
        canvas.width = screen.width;
        canvas.height = screen.height;
        this.platno = platno;
        this.context = this.platno.getContext("2d");
        this.model = new Model(this.platno, this.context, screen);
        this.eventListener(this.model);
        setInterval(this.controllerLoop, 1000 / 10, this);
    }

    controllerLoop(controller){
        controller.model.modelLoop();
    }
}