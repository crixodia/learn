use regex::Regex;

// Solve the calculator on exercise b with functions

fn solve_operation(input: String, op: &str) -> String {
    let mut input = input;
    let l_expr = r"(\d+)\s?\";
    let r_expr = r"\s?(\d+)";
    let expression = l_expr.to_owned() + if op == "/" { r"\" } else { "" } + op + r_expr;
    let re_op = Regex::new(expression.as_str()).unwrap();
    loop {
        let caps = re_op.captures(input.as_str());

        if caps.is_none() {
            break;
        }

        let caps = caps.unwrap();

        let cap_expression = caps.get(0).unwrap().as_str();

        let left_value: i32 = caps.get(1).unwrap().as_str().parse().unwrap();
        let right_value: i32 = caps.get(2).unwrap().as_str().parse().unwrap();

        let operation = match op {
            // Acts as a switch statement
            "+" => left_value + right_value,
            "-" => left_value - right_value,
            "*" => left_value * right_value,
            "/" => left_value / right_value,
            "^" => left_value.pow(right_value as u32),
            _ => 0,
        };

        input = input.replace(cap_expression, operation.to_string().as_str());
    }
    return input.parse().unwrap();
}

fn solve_all(input: String) -> String {
    let operations = ["^", "*", "/", "+", "-"];
    let mut result = input;
    for op in operations.iter() {
        result = solve_operation(result, op);
    }
    return result;
}

fn main() {
    println!("Enter an expression: ");
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let original = input.clone(); // Clone the input string

    println!("{} = {}", original.trim(), solve_all(input));
}
