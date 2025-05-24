from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
import subprocess

app = FastAPI()

class PositionRequest(BaseModel):
    position: str  # e.g., XGID or other format understood by GNUBG

@app.get("/")
def read_root():
    return {"status": "ok"}

@app.post("/best-move")
def get_best_move(req: PositionRequest):
    try:
        # Example: call gnubg in external mode with the provided position
        # This is a placeholder; adjust the command as needed for your setup
        process = subprocess.run(
            ["gnubg", "--external"],
            input=f"set board {req.position}\nshow moves\nquit\n",
            text=True,
            capture_output=True,
            timeout=10
        )
        if process.returncode != 0:
            raise Exception(process.stderr)
        # Parse output for best move (placeholder logic)
        output = process.stdout
        # You may want to parse the output more intelligently here
        return {"output": output}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e)) 