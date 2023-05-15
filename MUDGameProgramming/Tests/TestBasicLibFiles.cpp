#include <gtest/gtest.h>
#include <fstream>
#include <dirent.h>

#include "BasicLib/BasicLibFiles.h"

class BasicLibFilesTest : public testing::Test {
protected:
    void SetUp() override {
        // Cria um diretório temporário para os testes
        ASSERT_TRUE(mkdir(m_tempDirName.c_str(), 0700) == 0);

        // Cria dois arquivos dentro do diretório temporário
        std::ofstream file1(m_tempDirName + m_tempFilesName[0]);
        file1 << "Hello, world!";
        file1.close();

        std::ofstream file2(m_tempDirName + m_tempFilesName[1]);
        file2 << "Goodbye, world!";
        file2.close();
    }

    void TearDown() override {
        // Remove o diretório temporário com seus arquivos
        ASSERT_TRUE(remove((m_tempDirName + m_tempFilesName[0]).c_str()) == 0);
        ASSERT_TRUE(remove((m_tempDirName + m_tempFilesName[1]).c_str()) == 0);
        ASSERT_TRUE(rmdir(m_tempDirName.c_str()) == 0);
    }

    std::string m_tempDirName = "test_temp_dir/";
    std::string m_tempFilesName[2] = {"file1.txt", "file2.txt"};
};

TEST_F(BasicLibFilesTest, GetFileList) {
    // Chama a função GetFileList com o diretório temporário criado nos testes
    BasicLib::filelist fileList = BasicLib::GetFileList(m_tempDirName);

    // Verifica se a lista de arquivos contém os arquivos criados nos testes
    EXPECT_NE(fileList.find(m_tempDirName + m_tempFilesName[0]), fileList.end());
    EXPECT_NE(fileList.find(m_tempDirName + m_tempFilesName[1]), fileList.end());

    // Chama a função GetFileList com um diretório inexistente
    // BasicLib::filelist fileList = BasicLib::GetFileList("nonexistent_dir"); // Segmentation Fault
    FAIL() << "The call to BasicLib::GetFileList with a non existent directory "
              "results on Segmentation Fault (core dumped)\n";
    // Verifica se a lista de arquivos retornada está vazia
    // EXPECT_TRUE(fileList.empty());
}