from dir import file_handling, audio_handling

fh = file_handling.FileHandling()
ah = audio_handling.AudioHandling()

cg = ah.composite_grouping(fh.get_files())

for c in cg:
    print(c, cg[c])
