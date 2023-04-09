fn main() {
    println!("Hello cruel world!");
    /*
     *   Integers
     *   Length  Signed  Unsigned
     *   8-bit   i8      u8
     *   16-bit  i16     u16
     *   32-bit  i32     u32
     *   64-bit  i64     u64
     *   128-bit i128    u128
     *   arch    isize   usize
     */
    let yo: i8 = 26;
    let name: &str = "Cristian";

    println!("Hi I am {} and I am {}", name, yo);

    let max_temp: f32 = 23.5;
    let min_temp: f32 = -2.5;

    println!(
        "The max temperature is {} and the min temperature is {}",
        max_temp, min_temp
    );

    // These are inmutable variables
    // yo = 27; is not allowed

    // ----------------- Getting user input -----------------
    println!("Please enter your name: ");
    // Now this is a mutable variable, String What is the difference between String and &str?
    let mut user_name: String = "".to_string(); // Other ways: String::new() or String::from("")
    std::io::stdin().read_line(&mut user_name).unwrap(); // unwrap() can be used to manage errors

    println!("Please enter your age: ");
    let mut user_yo: String = "".to_string();
    std::io::stdin().read_line(&mut user_yo).unwrap();
    let user_yo_int: u8 = user_yo.trim().parse().unwrap(); // Parsing string to integer

    println!("Hello {}, you are {}", user_name.trim(), user_yo_int);

    // ----------------- Conditional statements -----------------
    // Conditional operators: ==, !=, >, <, >=, <=
    if user_yo_int >= 18 {
        println!("You are an adult, lets go to the bar!");
        if user_yo_int == 30 {
            println!("Have a free beer");
        }
    } else {
        println!("You are a minor, lets go to the park!");
    }

    // Boolean type
    let is_raining: bool = true;
    let is_sunny: bool = false;

    // Conditional statements
    // && -> and
    // || -> or
    // ! -> not
    if is_raining && is_sunny {
        println!("It is raining and sunny at the same time!");
    } else if is_raining {
        println!("It is raining!");
    } else if is_sunny {
        println!("It is sunny!");
    } else {
        println!("It is not raining nor sunny!");
    }

    // It is not mandatory to specify the type of the variable
    let num_1 = 10;
    let num_2 = 20;
    let sum = num_1 + num_2;

    //----------------- Loops -----------------
    loop {
        println!("What is {} + {}?", num_1, num_2);

        let mut user_answer: String = "".to_string();
        std::io::stdin().read_line(&mut user_answer).unwrap();
        let user_answer_int: i32 = user_answer.trim().parse().unwrap(); // For parsing it is necessary to specify the type

        if user_answer_int == sum {
            println!("You are right!");
            break; // Break the loop
        } else {
            println!("You are wrong!");
        }
    }
}
