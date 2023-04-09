use regex::Regex; // Import the regex crate
                  // Rust uses cargo to manage dependencies
                  // You can add dependencies to your Cargo.toml file by visiting https://crates.io/

// Rust does not have null values, because null values can cause errors
// That is why we use the Option<T> enum
// For instance, unwrap() returns the value of an Some variant, and panics if the value is a None variant
// If an error occurs, Rust returns a Result<T, E> enum. Where T is the return type and E is the error type
fn main() {
    let re_add = Regex::new(r"(\d+)\s?\+\s?(\d+)").unwrap();
    let re_sub = Regex::new(r"(\d+)\s?\-\s?(\d+)").unwrap();
    let re_div = Regex::new(r"(\d+)\s?/\s?(\d+)").unwrap();
    let re_exp = Regex::new(r"(\d+)\s?\^\s?(\d+)").unwrap();
    let re_mul = Regex::new(r"(\d+)\s?\*\s?(\d+)").unwrap();

    println!("Enter an expression: ");
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let original = input.clone(); // Clone the input string

    // Math operations order
    // 0. Parentheses
    // 1. Exponents
    // 2. Multiplication
    // 3. Division
    // 4. Addition
    // 5. Subtraction

    // Exponents
    loop {
        let caps = re_exp.captures(input.as_str());

        if caps.is_none() {
            break;
        }

        let caps = caps.unwrap();

        let cap_expression = caps.get(0).unwrap().as_str();

        let left_value: i32 = caps.get(1).unwrap().as_str().parse().unwrap();
        let right_value: i32 = caps.get(2).unwrap().as_str().parse().unwrap();

        let exp = left_value.pow(right_value as u32);

        input = input.replace(cap_expression, exp.to_string().as_str());
    }

    // Multiplication
    loop {
        let caps = re_mul.captures(input.as_str());

        if caps.is_none() {
            break;
        }

        let caps = caps.unwrap();

        let cap_expression = caps.get(0).unwrap().as_str();

        let left_value: i32 = caps.get(1).unwrap().as_str().parse().unwrap();
        let right_value: i32 = caps.get(2).unwrap().as_str().parse().unwrap();

        let multiplication = left_value * right_value;

        input = input.replace(cap_expression, multiplication.to_string().as_str());
    }

    // Integer division
    loop {
        let caps = re_div.captures(input.as_str());

        if caps.is_none() {
            break;
        }

        let caps = caps.unwrap();

        let cap_expression = caps.get(0).unwrap().as_str();

        let left_value: i32 = caps.get(1).unwrap().as_str().parse().unwrap();
        let right_value: i32 = caps.get(2).unwrap().as_str().parse().unwrap();

        let div = left_value / right_value;
        println!("Notice that this calculator doe not support floating point numbers.");

        input = input.replace(cap_expression, div.to_string().as_str());
    }

    // Addition
    loop {
        let caps = re_add.captures(input.as_str());

        if caps.is_none() {
            break;
        }

        let caps = caps.unwrap();

        let cap_expression = caps.get(0).unwrap().as_str();

        let left_value: i32 = caps.get(1).unwrap().as_str().parse().unwrap();
        let right_value: i32 = caps.get(2).unwrap().as_str().parse().unwrap();

        let addition = left_value + right_value;

        input = input.replace(cap_expression, addition.to_string().as_str());
    }

    // Subtraction
    loop {
        let caps = re_sub.captures(input.as_str());

        if caps.is_none() {
            break;
        }

        let caps = caps.unwrap();

        let cap_expression = caps.get(0).unwrap().as_str();

        let left_value: i32 = caps.get(1).unwrap().as_str().parse().unwrap();
        let right_value: i32 = caps.get(2).unwrap().as_str().parse().unwrap();

        let subtraction = left_value - right_value;

        input = input.replace(cap_expression, subtraction.to_string().as_str());
    }

    println!("{} = {}", original.trim(), input)
}
