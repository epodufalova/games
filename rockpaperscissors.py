import random

def get_choices():
    player_choice = input('Enter a choice(rock/paper/scissors): ')
    options = ['rock', 'paper', 'scissors']    
    computer_choice = random.choice(options)
    choices = {'player': player_choice, 'computer': computer_choice}
    return choices

def check_win(player, computer):
    print(f'You chose {player}, computer chose {computer}')
    if player == computer:
        return 'it is a tie'
    elif player == 'rock': 
         if computer == 'scissors': 
            return 'Rock smashes scissors! You win!'
         if computer == 'paper':
            return 'Paper covers rock! You lose!'
    elif player == 'paper':
        if computer == 'rock':
            return 'Paper covers rock! You win!'
        if computer == 'scissors':
            return 'Scissors cut paper! You lose!'
    elif player == 'scissors':
         if computer == 'rock':
            return 'Rock smashes scissors! You lose!'
         if computer == ' paper':
            return 'Scissors are cutting paper! You win!'

choices = get_choices()
result = check_win(choices['player'], choices['computer'])
print(result)