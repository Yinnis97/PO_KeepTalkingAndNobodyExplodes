from machine import Pin, I2C
import ssd1306
import time
import urandom

# Setup voor knoppen
button_up = Pin(14, Pin.IN, Pin.PULL_DOWN)
button_down = Pin(15, Pin.IN, Pin.PULL_DOWN)
button_select = Pin(16, Pin.IN, Pin.PULL_DOWN)

# Setup voor OLED
i2c = I2C(0, scl=Pin(5), sda=Pin(4))
oled = ssd1306.SSD1306_I2C(128, 64, i2c)

# Mogelijke wachtwoord opties
words = [
    ["a", "b", "c", "d", "e"],
    ["f", "g", "h", "i", "j"],
    ["k", "l", "m", "n", "o"],
    ["p", "q", "r", "s", "t"],
    ["u", "v", "w", "x", "y"]
]

# Lijst met geldige woorden (vijf letterwoorden)
valid_words = ["Stoom", "Blauw", "Grond", "Klopt", "Plank", "Plons"]

current_pos = [0, 0, 0, 0, 0]  # Huidige selectie per positie
current_index = 0  # Index van het karakter dat nu geselecteerd is
target_password = []  # Dit wordt het gegenereerde wachtwoord

# Functie om een willekeurig wachtwoord te genereren
def generate_random_password():
    return [urandom.randint(0, len(words[i]) - 1) for i in range(5)]

# Functie om van indices naar het daadwerkelijke woord om te zetten
def get_word_from_indices(indices):
    return "".join([words[i][indices[i]] for i in range(5)])

# Functie om te controleren of het gegenereerde wachtwoord een echt woord is
def is_valid_word(word):
    return word in valid_words

# Functie om een geldig willekeurig wachtwoord te genereren
def generate_valid_password():
    while True:
        indices = generate_random_password()
        word = get_word_from_indices(indices)
        if is_valid_word(word):
            return indices

# Functie om wachtwoord op OLED te tonen
def display_password():
    oled.fill(0)
    password = get_word_from_indices(current_pos)  # Huidige selectie
    oled.text("Password:", 0, 0)
    oled.text(password, 0, 20)
    oled.show()

# Functie om knoppen in te lezen
def check_buttons():
    global current_index
    if button_up.value() == 1:
        current_pos[current_index] = (current_pos[current_index] + 1) % len(words[current_index])
        time.sleep(0.2)  # Debouncing
    if button_down.value() == 1:
        current_pos[current_index] = (current_pos[current_index] - 1) % len(words[current_index])
        time.sleep(0.2)  # Debouncing

# Functie om de invoer te vergelijken met het gegenereerde wachtwoord
def check_password():
    return current_pos == target_password

# Main loop
def main():
    global target_password
    target_password = generate_valid_password()  # Genereer een geldig wachtwoord
    #print("Target password is:", get_word_from_indices(target_password))  # Voor debugging

    while True:
        check_buttons()
        display_password()

        # Controleer of de gebruiker klaar is
        if button_select.value() == 1:
            if check_password():
                oled.fill(0)
                oled.text("Correct!", 0, 20)
                oled.show()
            else:
                oled.fill(0)
                oled.text("Wrong PW!", 0, 20)
                oled.show()
            time.sleep(2)  # Wacht even voor feedback
            break  # Stop het spel

main()