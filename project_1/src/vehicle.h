class vehicle {
    private:
        // 4 bytes vehicle id
        unsigned int _id;

        // 2 bytes vehicle heading
        unsigned short _heading;
        
        // 6 bytes Vehicle size
        unsigned short _size_length;
        unsigned short _size_width;
        unsigned short _size_height;

    public:
        // constructor with full fields as parameters
        vehicle(unsigned int id, unsigned short heading, unsigned short size_length, unsigned short size_width, unsigned short size_height);

        // fields getters
        unsigned int id() const;             // 4 bytes vehicle id
        unsigned short heading() const;      // 2 bytes vehicle heading
        unsigned short size_length() const;  // 6 bytes Vehicle size - length
        unsigned short size_width() const;   // 6 bytes Vehicle size - width
        unsigned short size_height() const;  // 6 bytes Vehicle size - height
};
