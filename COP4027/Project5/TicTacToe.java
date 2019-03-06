import java.util.concurrent.locks.ReentrantLock;

public class TicTacToe
{
    private char[][] board;
    
    private boolean inProgress;
    
    private boolean gameOver;
    
    private int winner;
    
    private int turn;
    
    private ReentrantLock gameLock;
    
    private boolean[] playersReady;
    
    public TicTacToe()
    {
        this.inProgress = false;
        this.gameOver = false;
        this.winner = -1;
        this.turn = 0;
        this.playersReady = new boolean[2];
        this.playersReady[0] = false;
        this.playersReady[1] = false;
        this.gameLock = new ReentrantLock();
    }
    
    public boolean isValidPlayer(int player)
    {
        return (player == 1 || player == 2);
    }
    
    public boolean readyPlayer(int player)
    {
        if (isValidPlayer(player))
        {
            if (!playersReady[player - 1])
            {
                playersReady[player - 1] = true;
                
                return true;
            }
        }
        
        return false;
    }
    
    public boolean removePlayer(int player)
    {
        if (isValidPlayer(player))
        {
            this.playersReady[player - 1] = false;
            
            return true;
        }
        
        return false;
    }
    
    public boolean playerReady(int player)
    {
        if (isValidPlayer(player))
        {
            return playersReady[player - 1];
        }
        
        return false;
    }
    
    private void initializeBoard()
    {
        this.board = new char[3][3];
        
        for (int y = 0; y < 3; ++y)
        {
            for (int x = 0; x < 3; ++x)
            {
                this.board[y][x] = '_';
            }
        }
    }
    
    public boolean isPlayerTurn(int player)
    {
        return (turn == player);
    }
    
    public boolean isValidPosition(int x, int y)
    {
        if (x < 0 || x > 2)
        {
            return false;
        }
        
        if (y < 0 || y > 2)
        {
            return false;
        }
        
        return true;
    }
    
    public boolean isValidMove(int x, int y)
    {
        return board[y][x] == '_';
    }
    
    public boolean playerWon(int player)
    {
        char lookFor = (player == 1 ? 'X' : 'O');
        
        // Check if player has a full row
        for (int y = 0; y < 3; ++y)
        {
            if (board[y][0] == lookFor &&
                    board[y][1] == lookFor &&
                    board[y][2] == lookFor)
            {
                return true;
            }
        }
        
        // Check if player has a full column
        for (int x = 0; x < 3; ++x)
        {
            if (board[0][x] == lookFor &&
                    board[1][x] == lookFor &&
                    board[2][x] == lookFor)
            {
                return true;
            }
        }
        
        // Check for diagonal wins
        if (board[1][1] == lookFor)
        {
            // Check for top-left to bottom-right wins
            if (board[0][0] == lookFor && board[2][2] == lookFor)
            {
                return true;
            }
            
            // Check for top-right to bottom-left wins
            if (board[0][2] == lookFor && board[2][0] == lookFor)
            {
                return true;
            }
        }
        
        return false;
    }
    
    public boolean checkBoard()
    {
        if (playerWon(1))
        {
            this.winner = 1;
            this.turn = 0;
            this.inProgress = false;
            this.gameOver = true;
            
            return true;
        }
        else if (playerWon(2))
        {
            this.winner = 2;
            this.turn = 0;
            this.inProgress = false;
            this.gameOver = true;
            
            return true;
        }
        
        return false;
    }
    
    public int tryMove(int player, int x, int y)
    {
        int responseCode = 0;
        
        gameLock.lock();
        
        if (!gameOver)
        {
            if (inProgress)
            {
                if (playerReady(1) && playerReady(2))
                {
                    if (isPlayerTurn(player))
                    {
                        if (isValidPosition(x, y))
                        {
                            if (isValidMove(x, y))
                            {
                                board[y][x] = (player == 1 ? 'X' : 'O');
                                
                                checkBoard();
                                
                                // Flip the turn to opposing player
                                turn = (turn * 2) % 3;
                            }
                            else
                            {
                                responseCode = 6;
                            }
                        }
                        else
                        {
                            responseCode = 5;
                        }
                    }
                    else
                    {
                        responseCode = 4;
                    }
                }
                else
                {
                    responseCode = 3;
                }
            }
            else
            {
                responseCode = 2;
            }
        }
        else
        {
            responseCode = 1;
        }
        
        gameLock.unlock();
        
        return responseCode;
    }
    
    public boolean startGame()
    {
        boolean started = false;
        
        gameLock.lock();
        
        if (this.winner == -1)
        {
            if (!this.inProgress)
            {
                this.gameOver = false;
                this.inProgress = true;
                this.turn = 1;
                initializeBoard();
                
                started = true;
            }
        }
        else
        {
            this.gameOver = false;
            this.inProgress = true;
            this.winner = -1;
            this.turn = 1;
            initializeBoard();
            
            started = true;
        }
        
        gameLock.unlock();
        
        return started;
    }
    
    public boolean gameIsOver()
    {
        return gameOver;
    }
    
    public int getWinner()
    {
        return winner;
    }
    
    public String getGameState()
    {
        String ret = "";
        
        for (int y = 0; y < 2; ++y)
        {
            for (int x = 0; x < 2; ++x)
            {
                ret += board[y][x];
                ret += '|';
            }
            
            ret += board[y][2];
            
            ret += "\n";
        }
        
        for (int x = 0; x < 2; ++x)
        {
            if (board[2][x] == '_')
            {
                ret += " ";
            }
            else
            {
                ret += board[2][x];
            }
            
            ret += "|";
        }
        
        ret += (board[2][2] == '_' ? ' ' : board[2][2]);
        
        ret += "\n";
        
        return ret;
    }
}
