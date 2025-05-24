from fastapi.testclient import TestClient
from gnubg_api_server import app

client = TestClient(app)

def test_best_move_endpoint():
    # Use a simple/valid position string for your gnubg build
    payload = {"position": "4HPwATDgc/ABMA"}  # Standard starting position
    response = client.post("/best-move", json=payload)
    assert response.status_code == 200
    data = response.json()
    assert "output" in data
    assert isinstance(data["output"], str)
    # Optionally, check for expected content in the output
    # assert "move" in data["output"].lower() 