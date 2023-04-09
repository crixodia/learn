fn main() {
    let mut names: Vec<String> = Vec::new();
    loop {
        println!("Write a name (empty to exit):");
        let mut name = String::new();
        std::io::stdin().read_line(&mut name).unwrap();

        if name.trim().is_empty() {
            // Checks if the name is empty
            break;
        }

        names.push(name.trim().to_string()); // Adds the name to the vector
    }
    println!("Names: {:?}", names); // Prints the vector

    // Prints the names using the index
    for i in 0..names.len() {
        println!("Name {}: {}", i, names[i]);
    }

    // Capitalizes the names and uses the iterator
    for name in names.iter() {
        println!("Name: {}", name.to_uppercase());
    }
}
