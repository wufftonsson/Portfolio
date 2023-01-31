import random
import hangman_worldlist as wordlist
import hangman_art as art
import os

print(art.logo)

display= []
#haetaan sana sanalistassa randomilla.
chosen_word = random.choice(wordlist.word_list)

print(f"For testing purpose {chosen_word}")

#luotaan viivoja listaan yhtä monta kuin sana on pitkä.
for length in range(len(chosen_word)):
    
    display.append("_")

lives  = 6

end_of_game = False

#pelia toistetaan niin pitkään kunnes sana on valmis tai elämät loppuu.
while not end_of_game:
    guess = input("Guess a Letter: ").lower()

    #Tällä saadaan console clearattua jokaisen arvauksen jälkeen.
    os.system('cls')

    #Jos yrittää uudestaan jo arvattua sanaa 
    if guess in display:
      print(f"You already guessed {guess}, try something else! :)")

    #Loopissa käydään indexejä läpi, mikäli arvattu kirjain vastaa valitun sanan kirjainta. se tallentaan display listaan samaan indexiin mistä se löyty valitusta sanasta.
    for position in range(len(chosen_word)):
        letter = chosen_word[position]
        if guess == letter:
            display[position]=letter

    
      # Jos kirjainta ei löydy sanasta ollenkaan elämä lähtee.
    if guess not in chosen_word:
        print(f" Yoo guessed letter {guess}, that is not in the word! You lose a life ")
        lives -= 1


    #join funktiolla muutetaan lista järkevästi luettavaksi.
    print(f"{''.join(display)}")
    print("-------------------------------------------------\n")
    #Tarkistetaan onko peli voitettu tai hävitty.
    if "_" not in display:
        end_of_game = True
        print("You Win!")
    elif lives == 0:
        end_of_game= True
        print("You Lose")
    #Tuodaan art filusta elämien määrän mukanen hirressä kiikkuva ukko.
    print(art.stages[lives])


