use csv::{ReaderBuilder, StringRecord};
use std::collections::HashMap;
use std::fs;

const FILENAME: &str = "history.csv";

#[derive(Debug)]
struct HistoryData {
    type_h: String,
    tag: String,
    text: String,
    live: i32,
}

impl HistoryData {
    fn new(row: StringRecord) -> HistoryData {
        //let live: i32 = row.get(3).unwrap().parse().unwrap_or(0); // 0 is default value
        return HistoryData {
            type_h: row.get(0).unwrap().trim().to_string(),
            tag: row.get(1).unwrap().trim().to_string(),
            text: row.get(2).unwrap().trim().to_string(),
            live: 0,
        };
    }
}

fn main() {
    let content = fs::read_to_string(FILENAME).unwrap();
    let mut r = ReaderBuilder::new()
        .delimiter(b';')
        .from_reader(content.as_bytes());

    let mut history_data: HashMap<String, HistoryData> = HashMap::new();

    for result in r.records() {
        let row = result.unwrap();
        let history = HistoryData::new(row);
        history_data.insert(history.tag.clone(), history);
    }

    println!("{:?}", history_data["DERECHA"]);
}
