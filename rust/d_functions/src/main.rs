fn add(a: i32, b: i32) -> i32 {
    return a + b;
}

fn main() {
    let mut a: String = String::new();
    println!("Give a number");
    std::io::stdin().read_line(&mut a).unwrap();

    println!("Give another number");
    let mut b: String = String::new();
    std::io::stdin().read_line(&mut b).unwrap();

    let a: i32 = a.trim().parse().unwrap();
    let b: i32 = b.trim().parse().unwrap();
    println!("{} + {} = {}", a, b, add(a, b));
}
