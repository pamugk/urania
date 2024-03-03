fn main() {
    let args: Vec<String> = std::env::args().collect();
    if args.len() < 2 {
        println!("No arguments passed. Please pass path to OSM PBF file to be used.");
        return;
    }

    let reader = osmpbf::ElementReader::from_path(&args[1]);

    loop {
        println!("Please choose one of the commands below:\n\texit - Exit the program");
        let mut input = String::new();
        std::io::stdin()
            .read_line(&mut input)
            .expect("Failed to read input");
        match input.as_str() {
            "exit\n" => {
                println!("Exiting the program…");
                break;
            }
            _ => println!("Unknown command!"),
        }
    }
}
